from pathlib import Path

from sed_baseline.config import DEFAULT_AUDIO
from sed_baseline.dataset import SEDDataset
from sed_baseline.utils import load_event_classes


def main() -> None:
    root = Path(".")
    classes = load_event_classes(root / "class_mapping.json")
    ds = SEDDataset(root / "train" / "audio", root / "train" / "annotations.csv", classes)
    audio_id, x, y = ds[0]
    print("audio_id:", audio_id)
    print("x:", tuple(x.shape))
    print("y:", tuple(y.shape))
    print("n_frames(cfg):", DEFAULT_AUDIO.n_frames)


if __name__ == "__main__":
    main()
