from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np
import pandas as pd
import torch
from torch.utils.data import DataLoader
from tqdm import tqdm

from sed_baseline.config import DEFAULT_AUDIO
from sed_baseline.dataset import SEDDataset
from sed_baseline.decode import decode_events, round_events
from sed_baseline.model import CRNN
from sed_baseline.utils import get_device, load_event_classes


def _shift_time(x: torch.Tensor, shift: int) -> torch.Tensor:
    """Shift along time axis with zero padding (no wrap).

    x: (B, 1, n_mels, T)
    shift > 0 shifts to the right (later), shift < 0 shifts to the left (earlier).
    """
    if shift == 0:
        return x
    t = x.size(-1)
    if abs(shift) >= t:
        return torch.zeros_like(x)
    if shift > 0:
        pad = torch.zeros_like(x[..., :shift])
        return torch.cat([pad, x[..., : t - shift]], dim=-1)
    shift = -shift
    pad = torch.zeros_like(x[..., :shift])
    return torch.cat([x[..., shift:], pad], dim=-1)


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--data_root", type=str, default=".")
    ap.add_argument(
        "--ckpt",
        type=str,
        action="append",
        default=None,
        help="Checkpoint path. Can be provided multiple times for ensembling.",
    )
    ap.add_argument(
        "--use_ema",
        action="store_true",
        help="If checkpoint contains EMA weights, use them for inference.",
    )
    ap.add_argument(
        "--tta_shifts",
        type=int,
        default=0,
        help="Time-shift TTA in frames (100ms). 0=off. 1 uses shifts [-1,0,+1].",
    )
    ap.add_argument("--batch_size", type=int, default=16)
    ap.add_argument("--num_workers", type=int, default=0)
    ap.add_argument(
        "--threshold",
        type=float,
        default=None,
        help="Use a single scalar threshold for all classes (overrides checkpoint thresholds)",
    )
    ap.add_argument(
        "--threshold_mode",
        type=str,
        default="first",
        choices=["first", "avg"],
        help="When ensembling without --threshold: use thresholds from first checkpoint, or average thresholds across checkpoints.",
    )
    ap.add_argument("--min_event_frames", type=int, default=2)
    ap.add_argument("--merge_gap_frames", type=int, default=1)
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--no_id", action="store_true", help="Do NOT include 'id' column")
    g.add_argument("--with_id", action="store_true", help="Include 'id' column")
    ap.add_argument("--out", type=str, default="submission.csv")
    args = ap.parse_args()

    root = Path(args.data_root)
    device = get_device()

    ckpt_paths = args.ckpt if args.ckpt else ["checkpoints/crnn_best.pt"]
    ckpts = [torch.load(Path(p), map_location="cpu") for p in ckpt_paths]

    classes = ckpts[0].get("event_classes") or load_event_classes(root / "class_mapping.json")
    for i, c in enumerate(ckpts[1:], start=1):
        c_classes = c.get("event_classes") or load_event_classes(root / "class_mapping.json")
        if list(c_classes) != list(classes):
            raise ValueError(f"Checkpoint classes mismatch: {ckpt_paths[0]} vs {ckpt_paths[i]}")

    models = []
    for c in ckpts:
        m = CRNN(n_mels=DEFAULT_AUDIO.n_mels, n_classes=len(classes)).to(device)
        state = c.get("ema_state") if args.use_ema and ("ema_state" in c) else c["model_state"]
        m.load_state_dict(state, strict=True)
        m.eval()
        models.append(m)

    # Threshold selection
    if args.threshold is not None:
        thresholds = [float(args.threshold)] * len(classes)
    else:
        if args.threshold_mode == "avg":
            per_ckpt = []
            for c in ckpts:
                th = c.get("thresholds")
                if isinstance(th, list) and len(th) == len(classes):
                    per_ckpt.append([float(v) for v in th])
            if per_ckpt:
                arr = np.array(per_ckpt, dtype=np.float32)
                thresholds = [float(v) for v in arr.mean(axis=0).tolist()]
            else:
                scalar = float(ckpts[0].get("threshold", 0.5))
                thresholds = [scalar] * len(classes)
        else:
            th0 = ckpts[0].get("thresholds")
            if isinstance(th0, list) and len(th0) == len(classes):
                thresholds = [float(v) for v in th0]
            else:
                scalar = float(ckpts[0].get("threshold", 0.5))
                thresholds = [scalar] * len(classes)

    test_ds = SEDDataset(
        audio_dir=root / "test" / "audio",
        annotations_csv=None,
        event_classes=classes,
        cfg=DEFAULT_AUDIO,
    )
    test_loader = DataLoader(
        test_ds,
        batch_size=args.batch_size,
        shuffle=False,
        num_workers=args.num_workers,
        pin_memory=(device.type == "cuda"),
    )

    include_id = args.with_id or (not args.no_id)

    rows = []
    next_id = 1

    with torch.no_grad():
        for audio_ids, x, _ in tqdm(test_loader, desc="infer"):
            x = x.to(device)

            probs_sum = None
            n_avg = 0

            shifts = [0]
            if args.tta_shifts and args.tta_shifts > 0:
                k = int(args.tta_shifts)
                shifts = list(range(-k, k + 1))

            for model in models:
                for s in shifts:
                    x_s = _shift_time(x, s)
                    p = torch.sigmoid(model(x_s))  # (B, T, C)
                    # align back to original timeline
                    p = _shift_time(p.permute(0, 2, 1).unsqueeze(1), -s).squeeze(1).permute(0, 2, 1)
                    p = p.detach().cpu().numpy()
                    if probs_sum is None:
                        probs_sum = p
                    else:
                        probs_sum += p
                    n_avg += 1

            probs = probs_sum / max(1, n_avg)  # (B, T, C)

            for b, audio_id in enumerate(audio_ids):
                ev = decode_events(
                    audio_id=audio_id,
                    probs=probs[b],
                    event_classes=classes,
                    threshold=thresholds,
                    hop_seconds=DEFAULT_AUDIO.hop_seconds,
                    clip_seconds=DEFAULT_AUDIO.clip_seconds,
                    min_event_frames=args.min_event_frames,
                    merge_gap_frames=args.merge_gap_frames,
                )
                ev = round_events(ev, decimals=3)

                for e in ev:
                    if include_id:
                        rows.append(
                            {
                                "id": next_id,
                                "audio_id": e.audio_id,
                                "event_label": e.event_label,
                                "onset": f"{e.onset:.3f}",
                                "offset": f"{e.offset:.3f}",
                            }
                        )
                        next_id += 1
                    else:
                        rows.append(
                            {
                                "audio_id": e.audio_id,
                                "event_label": e.event_label,
                                "onset": f"{e.onset:.3f}",
                                "offset": f"{e.offset:.3f}",
                            }
                        )

    df = pd.DataFrame(rows)

    out_path = Path(args.out)
    df.to_csv(out_path, index=False)
    print(f"Wrote {len(df)} rows to {out_path}")


if __name__ == "__main__":
    main()
