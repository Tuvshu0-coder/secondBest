from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np
import pandas as pd
import torch
from torch.utils.data import DataLoader
from tqdm import tqdm

from sed_baseline.config import DEFAULT_AUDIO
from sed_baseline.augment import SpecAugmentConfig, mixup_batch, spec_augment
from sed_baseline.dataset import SEDDataset
from sed_baseline.metrics import segment_macro_f1, segment_macro_f1_per_class_thresholds
from sed_baseline.model import CRNN
from sed_baseline.utils import get_device, load_event_classes, set_seed


def focal_bce_with_logits(
    logits: torch.Tensor,
    targets: torch.Tensor,
    pos_weight: torch.Tensor | None = None,
    gamma: float = 0.0,
) -> torch.Tensor:
    """Multi-label focal loss built on BCEWithLogits.

    gamma=0 -> standard BCE.
    """
    bce = torch.nn.functional.binary_cross_entropy_with_logits(
        logits,
        targets,
        reduction="none",
        pos_weight=pos_weight,
    )
    if gamma <= 0:
        return bce.mean()
    probs = torch.sigmoid(logits)
    p_t = probs * targets + (1 - probs) * (1 - targets)
    mod = (1 - p_t).pow(gamma)
    return (mod * bce).mean()


@torch.no_grad()
def tune_thresholds(
    probs: torch.Tensor,
    targets: torch.Tensor,
    grid: torch.Tensor,
) -> tuple[torch.Tensor, float, list[float]]:
    """Greedy per-class threshold tuning to maximize macro F1."""
    c = probs.size(-1)
    best = torch.full((c,), 0.5, dtype=torch.float32)

    # Coordinate descent over classes
    for _ in range(2):
        for ci in range(c):
            best_ci = best[ci].item()
            best_score = -1.0
            for thr in grid.tolist():
                cand = best.clone()
                cand[ci] = float(thr)
                res = segment_macro_f1_per_class_thresholds(probs, targets, cand)
                if res.macro_f1 > best_score:
                    best_score = res.macro_f1
                    best_ci = float(thr)
            best[ci] = best_ci

    final = segment_macro_f1_per_class_thresholds(probs, targets, best)
    return best, float(final.macro_f1), final.per_class_f1


def compute_pos_weight(
    annotations_csvs: list[Path],
    event_classes: list[str],
    n_frames: int,
    hop_seconds: float,
    n_audio: int,
) -> torch.Tensor:
    dfs = [pd.read_csv(p) for p in annotations_csvs]
    df = pd.concat(dfs, axis=0, ignore_index=True) if len(dfs) > 1 else dfs[0]
    pos = {c: 0.0 for c in event_classes}
    for row in df.itertuples(index=False):
        label = getattr(row, "event_label")
        onset = float(getattr(row, "onset"))
        offset = float(getattr(row, "offset"))
        if label not in pos:
            continue
        if offset <= onset:
            continue
        pos_frames = int(np.ceil((offset - onset) / hop_seconds))
        pos[label] += pos_frames

    total = float(n_audio * n_frames)
    weights = []
    for c in event_classes:
        p = float(pos[c])
        if p <= 0:
            weights.append(1.0)
        else:
            neg = max(1.0, total - p)
            weights.append(neg / p)
    return torch.tensor(weights, dtype=torch.float32)


class EMA:
    def __init__(self, model: torch.nn.Module, decay: float = 0.999) -> None:
        if not (0.0 < decay < 1.0):
            raise ValueError("EMA decay must be in (0, 1)")
        self.decay = float(decay)
        self.shadow = {k: v.detach().clone() for k, v in model.state_dict().items()}
        self._backup: dict[str, torch.Tensor] | None = None

    @torch.no_grad()
    def update(self, model: torch.nn.Module) -> None:
        msd = model.state_dict()
        for k, v in msd.items():
            if k not in self.shadow:
                self.shadow[k] = v.detach().clone()
                continue
            # Some buffers are integer (e.g., BatchNorm.num_batches_tracked). Don't EMA those.
            if not torch.is_floating_point(v):
                self.shadow[k] = v.detach().clone()
                continue

            # Keep dtypes consistent (important with AMP)
            if not torch.is_floating_point(self.shadow[k]):
                self.shadow[k] = v.detach().clone()
                continue

            self.shadow[k].mul_(self.decay).add_(v.detach(), alpha=1.0 - self.decay)

    def apply_to(self, model: torch.nn.Module) -> None:
        if self._backup is not None:
            raise RuntimeError("EMA is already applied")
        self._backup = {k: v.detach().clone() for k, v in model.state_dict().items()}
        model.load_state_dict(self.shadow, strict=False)

    def restore(self, model: torch.nn.Module) -> None:
        if self._backup is None:
            return
        model.load_state_dict(self._backup, strict=False)
        self._backup = None


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--data_root", type=str, default=".")
    ap.add_argument("--epochs", type=int, default=15)
    ap.add_argument("--batch_size", type=int, default=16)
    ap.add_argument("--lr", type=float, default=1e-3)
    ap.add_argument("--weight_decay", type=float, default=1e-2)
    ap.add_argument("--accum_steps", type=int, default=1, help="Gradient accumulation steps")
    ap.add_argument("--seed", type=int, default=42)
    ap.add_argument("--num_workers", type=int, default=0)
    ap.add_argument(
        "--train_batches",
        type=int,
        default=0,
        help="If >0, run only this many train batches per epoch (sanity check)",
    )
    ap.add_argument(
        "--val_batches",
        type=int,
        default=0,
        help="If >0, run only this many val batches per epoch (sanity check)",
    )
    ap.add_argument("--threshold", type=float, default=0.5, help="Reporting threshold (scalar)")
    ap.add_argument("--tune_thresholds", action="store_true", help="Tune per-class thresholds on validation")
    ap.add_argument("--focal_gamma", type=float, default=1.5)
    ap.add_argument("--specaug", action="store_true")
    ap.add_argument("--mixup_alpha", type=float, default=0.2)
    ap.add_argument(
        "--train_use_val",
        action="store_true",
        help="Include validation set in training (often improves leaderboard); note: this leaks validation into training.",
    )
    ap.add_argument("--ema_decay", type=float, default=0.999)
    ap.add_argument(
        "--eval_with_ema",
        action="store_true",
        help="Evaluate/checkpoint using EMA weights (recommended).",
    )
    ap.add_argument("--out", type=str, default="checkpoints/crnn_best.pt")
    args = ap.parse_args()

    set_seed(args.seed)
    device = get_device()

    root = Path(args.data_root)
    classes = load_event_classes(root / "class_mapping.json")

    train_ds = SEDDataset(
        audio_dir=root / "train" / "audio",
        annotations_csv=root / "train" / "annotations.csv",
        event_classes=classes,
        cfg=DEFAULT_AUDIO,
    )
    val_ds = SEDDataset(
        audio_dir=root / "validation" / "audio",
        annotations_csv=root / "validation" / "annotations.csv",
        event_classes=classes,
        cfg=DEFAULT_AUDIO,
    )

    if args.train_use_val:
        from torch.utils.data import ConcatDataset

        train_data = ConcatDataset([train_ds, val_ds])
        train_ann = [root / "train" / "annotations.csv", root / "validation" / "annotations.csv"]
        n_audio_for_weight = len(train_ds) + len(val_ds)
    else:
        train_data = train_ds
        train_ann = [root / "train" / "annotations.csv"]
        n_audio_for_weight = len(train_ds)

    train_loader = DataLoader(
        train_data,
        batch_size=args.batch_size,
        shuffle=True,
        num_workers=args.num_workers,
        pin_memory=(device.type == "cuda"),
    )
    val_loader = DataLoader(
        val_ds,
        batch_size=args.batch_size,
        shuffle=False,
        num_workers=args.num_workers,
        pin_memory=(device.type == "cuda"),
    )

    model = CRNN(n_mels=DEFAULT_AUDIO.n_mels, n_classes=len(classes)).to(device)

    ema = EMA(model, decay=args.ema_decay)

    pos_weight = compute_pos_weight(
        annotations_csvs=train_ann,
        event_classes=classes,
        n_frames=DEFAULT_AUDIO.n_frames,
        hop_seconds=DEFAULT_AUDIO.hop_seconds,
        n_audio=n_audio_for_weight,
    ).to(device)

    opt = torch.optim.AdamW(model.parameters(), lr=args.lr, weight_decay=args.weight_decay)
    sched = torch.optim.lr_scheduler.CosineAnnealingLR(opt, T_max=max(1, args.epochs))
    scaler = torch.amp.GradScaler("cuda", enabled=(device.type == "cuda"))
    spec_cfg = SpecAugmentConfig()

    # Thresholds (optionally tuned)
    best_thresholds = torch.full((len(classes),), float(args.threshold), dtype=torch.float32)

    best_f1 = -1.0
    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    for epoch in range(1, args.epochs + 1):
        model.train()
        tr_loss = 0.0
        n_batches = 0
        opt.zero_grad(set_to_none=True)
        for _, x, y in tqdm(train_loader, desc=f"train {epoch}"):
            x = x.to(device)
            y = y.to(device)  # (B, T, C)

            if args.specaug:
                x = spec_augment(x, spec_cfg)
            if args.mixup_alpha and args.mixup_alpha > 0:
                x, y = mixup_batch(x, y, alpha=args.mixup_alpha)

            with torch.amp.autocast("cuda", enabled=(device.type == "cuda")):
                logits = model(x)
                loss = focal_bce_with_logits(logits, y, pos_weight=pos_weight, gamma=args.focal_gamma)

            loss_scaled = loss / max(1, args.accum_steps)
            scaler.scale(loss_scaled).backward()

            if (n_batches + 1) % max(1, args.accum_steps) == 0:
                scaler.unscale_(opt)
                torch.nn.utils.clip_grad_norm_(model.parameters(), 5.0)
                scaler.step(opt)
                scaler.update()
                opt.zero_grad(set_to_none=True)
                ema.update(model)

            tr_loss += float(loss.item())
            n_batches += 1

            if args.train_batches and n_batches >= args.train_batches:
                break

        # If epoch ended mid-accumulation, still step once.
        if args.accum_steps > 1 and (n_batches % args.accum_steps) != 0:
            scaler.unscale_(opt)
            torch.nn.utils.clip_grad_norm_(model.parameters(), 5.0)
            scaler.step(opt)
            scaler.update()
            opt.zero_grad(set_to_none=True)
            ema.update(model)

        tr_loss = tr_loss / max(1, n_batches)

        model.eval()
        if args.eval_with_ema:
            ema.apply_to(model)
        all_probs = []
        all_tgts = []
        with torch.no_grad():
            v_batches = 0
            for _, x, y in tqdm(val_loader, desc=f"val {epoch}"):
                x = x.to(device)
                y = y.to(device)
                logits = model(x)
                probs = torch.sigmoid(logits)
                all_probs.append(probs.cpu())
                all_tgts.append(y.cpu())

                v_batches += 1
                if args.val_batches and v_batches >= args.val_batches:
                    break

        probs = torch.cat(all_probs, dim=0)
        tgts = torch.cat(all_tgts, dim=0)
        f1 = segment_macro_f1(probs, tgts, threshold=args.threshold)

        if args.tune_thresholds:
            grid = torch.linspace(0.05, 0.95, steps=19)
            tuned, tuned_macro, tuned_per_class = tune_thresholds(probs, tgts, grid)
            best_thresholds = tuned
            print(
                f"tuned thresholds={['%.2f' % v for v in best_thresholds.tolist()]} tuned_macro_f1={tuned_macro:.4f} "
                f"tuned_per_class={['%.3f' % v for v in tuned_per_class]}"
            )

        sched.step()

        if args.eval_with_ema:
            ema.restore(model)

        print(
            f"epoch={epoch} loss={tr_loss:.4f} val_macro_f1={f1.macro_f1:.4f} "
            f"per_class={['%.3f' % v for v in f1.per_class_f1]}"
        )

        score_for_ckpt = f1.macro_f1
        if args.tune_thresholds:
            tuned_res = segment_macro_f1_per_class_thresholds(probs, tgts, best_thresholds)
            score_for_ckpt = tuned_res.macro_f1

        if score_for_ckpt > best_f1:
            best_f1 = score_for_ckpt
            torch.save(
                {
                    "model_state": model.state_dict(),
                    "ema_state": ema.shadow,
                    "used_ema_for_eval": bool(args.eval_with_ema),
                    "event_classes": classes,
                    "audio_cfg": DEFAULT_AUDIO.__dict__,
                    "threshold": float(args.threshold),
                    "thresholds": best_thresholds.tolist(),
                },
                out_path,
            )
            print(f"Saved best checkpoint to {out_path} (macro_f1={best_f1:.4f})")


if __name__ == "__main__":
    main()
