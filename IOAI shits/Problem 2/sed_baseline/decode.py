from __future__ import annotations

from dataclasses import dataclass
from typing import Iterable, List

import numpy as np


@dataclass(frozen=True)
class Event:
    audio_id: str
    event_label: str
    onset: float
    offset: float


def _merge_short_gaps(mask: np.ndarray, gap_frames: int) -> np.ndarray:
    if gap_frames <= 0:
        return mask
    out = mask.copy()
    false_runs = []
    in_run = False
    start = 0
    for i, v in enumerate(out):
        if (not v) and not in_run:
            in_run = True
            start = i
        if v and in_run:
            false_runs.append((start, i))
            in_run = False
    if in_run:
        false_runs.append((start, len(out)))

    for s, e in false_runs:
        if e - s <= gap_frames:
            out[s:e] = True
    return out


def _remove_short_events(mask: np.ndarray, min_event_frames: int) -> np.ndarray:
    if min_event_frames <= 1:
        return mask
    out = mask.copy()
    true_runs = []
    in_run = False
    start = 0
    for i, v in enumerate(out):
        if v and not in_run:
            in_run = True
            start = i
        if (not v) and in_run:
            true_runs.append((start, i))
            in_run = False
    if in_run:
        true_runs.append((start, len(out)))

    for s, e in true_runs:
        if e - s < min_event_frames:
            out[s:e] = False
    return out


def decode_events(
    audio_id: str,
    probs: np.ndarray,
    event_classes: List[str],
    threshold: float | List[float] = 0.5,
    hop_seconds: float = 0.1,
    clip_seconds: float = 15.0,
    min_event_frames: int = 1,
    merge_gap_frames: int = 0,
) -> List[Event]:
    """Convert framewise probabilities (T, C) into event list."""
    if probs.ndim != 2:
        raise ValueError("probs must be (T, C)")

    t_frames, n_classes = probs.shape
    if n_classes != len(event_classes):
        raise ValueError("probs classes mismatch")

    if isinstance(threshold, list):
        if len(threshold) != len(event_classes):
            raise ValueError("threshold list must match number of classes")
        thresholds = threshold
    else:
        thresholds = [float(threshold)] * len(event_classes)

    events: list[Event] = []
    for c, label in enumerate(event_classes):
        mask = probs[:, c] >= float(thresholds[c])
        mask = _merge_short_gaps(mask, merge_gap_frames)
        mask = _remove_short_events(mask, min_event_frames)

        in_event = False
        start = 0
        for i, v in enumerate(mask):
            if v and not in_event:
                in_event = True
                start = i
            if (not v) and in_event:
                end = i
                onset = start * hop_seconds
                offset = end * hop_seconds
                onset = float(np.clip(onset, 0.0, clip_seconds))
                offset = float(np.clip(offset, 0.0, clip_seconds))
                if offset > onset:
                    events.append(Event(audio_id, label, onset, offset))
                in_event = False
        if in_event:
            end = t_frames
            onset = start * hop_seconds
            offset = end * hop_seconds
            onset = float(np.clip(onset, 0.0, clip_seconds))
            offset = float(np.clip(offset, 0.0, clip_seconds))
            if offset > onset:
                events.append(Event(audio_id, label, onset, offset))

    return events


def round_events(events: Iterable[Event], decimals: int = 3) -> list[Event]:
    out = []
    for e in events:
        onset = round(float(e.onset), decimals)
        offset = round(float(e.offset), decimals)
        if offset <= onset:
            continue
        out.append(Event(e.audio_id, e.event_label, onset, offset))
    return out
