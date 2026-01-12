from __future__ import annotations

import math
from dataclasses import dataclass

import torch


@dataclass(frozen=True)
class SpecAugmentConfig:
    freq_mask_param: int = 12
    time_mask_param: int = 24
    num_freq_masks: int = 2
    num_time_masks: int = 2


def spec_augment(x: torch.Tensor, cfg: SpecAugmentConfig) -> torch.Tensor:
    """Apply SpecAugment to log-mel spectrograms.

    x: (B, 1, M, T)
    """
    if x.dim() != 4:
        raise ValueError("x must be (B, 1, M, T)")

    b, _, m, t = x.shape
    out = x.clone()

    # Frequency masks
    for _ in range(cfg.num_freq_masks):
        f = torch.randint(low=0, high=max(1, cfg.freq_mask_param + 1), size=(b,), device=x.device)
        f0 = torch.randint(low=0, high=max(1, m), size=(b,), device=x.device)
        for i in range(b):
            width = int(min(f[i].item(), m))
            start = int(min(f0[i].item(), max(0, m - width)))
            if width > 0:
                out[i, :, start : start + width, :] = 0

    # Time masks
    for _ in range(cfg.num_time_masks):
        tt = torch.randint(low=0, high=max(1, cfg.time_mask_param + 1), size=(b,), device=x.device)
        t0 = torch.randint(low=0, high=max(1, t), size=(b,), device=x.device)
        for i in range(b):
            width = int(min(tt[i].item(), t))
            start = int(min(t0[i].item(), max(0, t - width)))
            if width > 0:
                out[i, :, :, start : start + width] = 0

    return out


def mixup_batch(
    x: torch.Tensor,
    y: torch.Tensor,
    alpha: float,
) -> tuple[torch.Tensor, torch.Tensor]:
    """MixUp for spectrogram batches.

    x: (B, 1, M, T)
    y: (B, T, C)
    """
    if alpha <= 0:
        return x, y

    b = x.size(0)
    perm = torch.randperm(b, device=x.device)

    lam = torch.distributions.Beta(alpha, alpha).sample((b,)).to(x.device)
    lam_x = lam.view(b, 1, 1, 1)
    lam_y = lam.view(b, 1, 1)

    x_mix = lam_x * x + (1 - lam_x) * x[perm]
    y_mix = lam_y * y + (1 - lam_y) * y[perm]
    return x_mix, y_mix
