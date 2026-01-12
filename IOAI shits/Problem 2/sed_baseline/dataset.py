from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Tuple

import numpy as np
import pandas as pd
import torch
import soundfile as sf
import torchaudio
from torch.utils.data import Dataset

from .config import AudioConfig, DEFAULT_AUDIO


def _ensure_mono(waveform: torch.Tensor) -> torch.Tensor:
    # waveform: (channels, samples)
    if waveform.dim() != 2:
        raise ValueError(f"Expected waveform shape (C, N), got {tuple(waveform.shape)}")
    if waveform.size(0) == 1:
        return waveform
    return waveform.mean(dim=0, keepdim=True)


def _pad_or_crop(waveform: torch.Tensor, n_samples: int) -> torch.Tensor:
    # waveform: (1, N)
    n = waveform.size(1)
    if n == n_samples:
        return waveform
    if n > n_samples:
        return waveform[:, :n_samples]
    pad = n_samples - n
    return torch.nn.functional.pad(waveform, (0, pad))


def _load_wav_soundfile(path: Path) -> tuple[torch.Tensor, int]:
    data, sr = sf.read(str(path), always_2d=True)
    # data: (N, C)
    data = data.astype(np.float32, copy=False)
    if data.shape[1] == 1:
        mono = data[:, 0]
    else:
        mono = data.mean(axis=1)
    waveform = torch.from_numpy(mono).unsqueeze(0)  # (1, N)
    return waveform, int(sr)


def _frame_targets_from_events(
    events: pd.DataFrame,
    class_to_idx: Dict[str, int],
    cfg: AudioConfig,
) -> torch.Tensor:
    y = torch.zeros((cfg.n_frames, len(class_to_idx)), dtype=torch.float32)
    hop = cfg.hop_seconds

    for row in events.itertuples(index=False):
        label = getattr(row, "event_label")
        onset = float(getattr(row, "onset"))
        offset = float(getattr(row, "offset"))

        if label not in class_to_idx:
            continue

        onset = max(0.0, min(cfg.clip_seconds, onset))
        offset = max(0.0, min(cfg.clip_seconds, offset))
        if offset <= onset:
            continue

        start = int(np.floor(onset / hop))
        end = int(np.ceil(offset / hop))
        start = max(0, min(cfg.n_frames, start))
        end = max(0, min(cfg.n_frames, end))
        if end <= start:
            continue

        y[start:end, class_to_idx[label]] = 1.0

    return y


@dataclass(frozen=True)
class SEDSample:
    audio_id: str
    x: torch.Tensor  # (1, n_mels, T)
    y: Optional[torch.Tensor]  # (T, C)


class SEDDataset(Dataset):
    def __init__(
        self,
        audio_dir: str | Path,
        annotations_csv: Optional[str | Path],
        event_classes: List[str],
        cfg: AudioConfig = DEFAULT_AUDIO,
        normalize: bool = True,
    ) -> None:
        self.audio_dir = Path(audio_dir)
        self.cfg = cfg
        self.normalize = normalize

        self.event_classes = list(event_classes)
        self.class_to_idx = {c: i for i, c in enumerate(self.event_classes)}

        self.audio_ids = sorted([p.stem for p in self.audio_dir.glob("*.wav")])
        if not self.audio_ids:
            raise FileNotFoundError(f"No .wav files found in {self.audio_dir}")

        self.annotations = None
        if annotations_csv is not None:
            df = pd.read_csv(annotations_csv)
            required = {"audio_id", "event_label", "onset", "offset"}
            missing = required - set(df.columns)
            if missing:
                raise ValueError(f"annotations.csv missing columns: {sorted(missing)}")
            self.annotations = df

        self.mel = torchaudio.transforms.MelSpectrogram(
            sample_rate=cfg.sample_rate,
            n_fft=cfg.n_fft,
            win_length=cfg.win_length,
            hop_length=cfg.hop_length,
            f_min=cfg.f_min,
            f_max=cfg.f_max,
            n_mels=cfg.n_mels,
            center=False,
            power=2.0,
        )
        self.db = torchaudio.transforms.AmplitudeToDB(stype="power", top_db=80.0)

    def __len__(self) -> int:
        return len(self.audio_ids)

    def __getitem__(self, idx: int) -> Tuple[str, torch.Tensor, Optional[torch.Tensor]]:
        audio_id = self.audio_ids[idx]
        wav_path = self.audio_dir / f"{audio_id}.wav"

        # Use soundfile to avoid torchaudio.load -> torchcodec dependency.
        waveform, sr = _load_wav_soundfile(wav_path)

        if sr != self.cfg.sample_rate:
            waveform = torchaudio.functional.resample(waveform, sr, self.cfg.sample_rate)

        waveform = _pad_or_crop(waveform, self.cfg.n_samples)

        x = self.db(self.mel(waveform))  # (1, n_mels, T)
        # Ensure expected frame count
        if x.size(-1) != self.cfg.n_frames:
            # Best-effort fix via crop/pad
            if x.size(-1) > self.cfg.n_frames:
                x = x[..., : self.cfg.n_frames]
            else:
                x = torch.nn.functional.pad(x, (0, self.cfg.n_frames - x.size(-1)))

        if self.normalize:
            mean = x.mean()
            std = x.std().clamp_min(1e-6)
            x = (x - mean) / std

        if self.annotations is not None:
            events = self.annotations[self.annotations["audio_id"] == audio_id]
            y = _frame_targets_from_events(events, self.class_to_idx, self.cfg)
        else:
            # Return a dummy tensor so default_collate works for test data.
            y = torch.zeros((self.cfg.n_frames, len(self.class_to_idx)), dtype=torch.float32)

        return audio_id, x, y
