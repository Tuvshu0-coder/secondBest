from __future__ import annotations

import os
from pathlib import Path
from zipfile import ZIP_STORED, ZipFile


EXCLUDE_DIRS = {
    ".venv",
    "__pycache__",
    ".git",
    ".ipynb_checkpoints",
    "checkpoints",
}

EXCLUDE_SUFFIXES = {
    ".pyc",
    ".pyo",
}

EXCLUDE_FILES = {
    "submission.csv",
    "submission_sanity.csv",
    "submission_adv_sanity.csv",
    "submission_with_id.csv",
}


def should_skip(path: Path) -> bool:
    name = path.name
    if name in EXCLUDE_FILES:
        return True
    if path.suffix.lower() in EXCLUDE_SUFFIXES:
        return True
    return False


def main() -> None:
    root = Path(__file__).resolve().parent
    out_zip = root / "kaggle_upload.zip"

    if out_zip.exists():
        out_zip.unlink()

    def iter_files() -> list[Path]:
        files: list[Path] = []
        for dirpath, dirnames, filenames in os.walk(root):
            dp = Path(dirpath)

            # prune excluded dirs in-place
            dirnames[:] = [
                d
                for d in dirnames
                if d not in EXCLUDE_DIRS and not d.endswith("__pycache__")
            ]

            for fn in filenames:
                p = dp / fn
                rel = p.relative_to(root)

                # never include the output zip itself
                if rel == out_zip.name:
                    continue

                if any(part in EXCLUDE_DIRS for part in rel.parts):
                    continue

                if should_skip(p):
                    continue

                files.append(p)
        return files

    files = iter_files()
    print(f"Zipping {len(files)} files into {out_zip}")

    # Use ZIP_STORED for speed; wav files don’t compress much anyway.
    with ZipFile(out_zip, "w", compression=ZIP_STORED, allowZip64=True) as z:
        for p in files:
            rel = p.relative_to(root).as_posix()
            z.write(p, arcname=rel)

    size_mb = out_zip.stat().st_size / (1024 * 1024)
    print(f"Done: {out_zip} ({size_mb:.1f} MB)")


if __name__ == "__main__":
    main()
