from __future__ import annotations

import json
import os
import random
from pathlib import Path
from typing import List

import numpy as np
import torch


def set_seed(seed: int) -> None:
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False


def get_device() -> torch.device:
    return torch.device("cuda" if torch.cuda.is_available() else "cpu")


def load_event_classes(class_mapping_path: str | os.PathLike) -> List[str]:
    with open(class_mapping_path, "r", encoding="utf-8") as f:
        data = json.load(f)
    classes = data.get("event_classes")
    if not classes:
        raise ValueError("class_mapping.json missing 'event_classes'")
    return list(classes)


def list_audio_ids(audio_dir: str | os.PathLike) -> List[str]:
    audio_dir = Path(audio_dir)
    wavs = sorted(audio_dir.glob("*.wav"))
    return [w.stem for w in wavs]
