from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class AudioConfig:
    sample_rate: int = 22050
    clip_seconds: float = 15.0
    hop_seconds: float = 0.1

    n_fft: int = 2048
    win_length: int = 2048
    hop_length: int = 2205  # 0.1s * 22050Hz

    n_mels: int = 64
    f_min: float = 20.0
    f_max: float = 11025.0

    @property
    def n_samples(self) -> int:
        return int(round(self.sample_rate * self.clip_seconds))

    @property
    def n_frames(self) -> int:
        # With center=False: floor((n_samples - n_fft)/hop) + 1
        return int((self.n_samples - self.n_fft) // self.hop_length + 1)


DEFAULT_AUDIO = AudioConfig()
