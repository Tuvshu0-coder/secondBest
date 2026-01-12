from __future__ import annotations

from dataclasses import dataclass

import torch


@dataclass(frozen=True)
class F1Result:
    macro_f1: float
    per_class_f1: list[float]


@torch.no_grad()
def segment_macro_f1(
    probs: torch.Tensor,
    targets: torch.Tensor,
    threshold: float = 0.5,
) -> F1Result:
    """Segment-based F1 over frames.

    probs: (N, T, C) or (T, C)
    targets: same shape as probs (binary 0/1)
    """
    if probs.dim() == 2:
        probs = probs.unsqueeze(0)
    if targets.dim() == 2:
        targets = targets.unsqueeze(0)

    pred = (probs >= threshold).to(torch.int64)
    tgt = targets.to(torch.int64)

    # Flatten over N and T
    pred = pred.reshape(-1, pred.size(-1))
    tgt = tgt.reshape(-1, tgt.size(-1))

    per_class = []
    for c in range(pred.size(-1)):
        p = pred[:, c]
        t = tgt[:, c]
        tp = int(((p == 1) & (t == 1)).sum().item())
        fp = int(((p == 1) & (t == 0)).sum().item())
        fn = int(((p == 0) & (t == 1)).sum().item())
        denom = 2 * tp + fp + fn
        f1 = 0.0 if denom == 0 else (2.0 * tp) / denom
        per_class.append(float(f1))

    macro = float(sum(per_class) / len(per_class)) if per_class else 0.0
    return F1Result(macro_f1=macro, per_class_f1=per_class)


@torch.no_grad()
def segment_macro_f1_per_class_thresholds(
    probs: torch.Tensor,
    targets: torch.Tensor,
    thresholds: torch.Tensor,
) -> F1Result:
    """Segment-based F1 where each class uses its own threshold.

    thresholds: (C,) float tensor
    """
    if probs.dim() == 2:
        probs = probs.unsqueeze(0)
    if targets.dim() == 2:
        targets = targets.unsqueeze(0)

    if thresholds.dim() != 1 or thresholds.numel() != probs.size(-1):
        raise ValueError("thresholds must be shape (C,)")

    thr = thresholds.view(1, 1, -1).to(probs.device)
    pred = (probs >= thr).to(torch.int64)
    tgt = targets.to(torch.int64)

    pred = pred.reshape(-1, pred.size(-1))
    tgt = tgt.reshape(-1, tgt.size(-1))

    per_class = []
    for c in range(pred.size(-1)):
        p = pred[:, c]
        t = tgt[:, c]
        tp = int(((p == 1) & (t == 1)).sum().item())
        fp = int(((p == 1) & (t == 0)).sum().item())
        fn = int(((p == 0) & (t == 1)).sum().item())
        denom = 2 * tp + fp + fn
        f1 = 0.0 if denom == 0 else (2.0 * tp) / denom
        per_class.append(float(f1))

    macro = float(sum(per_class) / len(per_class)) if per_class else 0.0
    return F1Result(macro_f1=macro, per_class_f1=per_class)
