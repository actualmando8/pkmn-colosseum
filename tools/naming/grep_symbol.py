#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SCAN_DIRS = ["src", "include", "config/GC6E01/symbols.txt"]
IGNORE_PARTS = {"archive", "build", ".git", "__pycache__"}

def should_scan(path: Path) -> bool:
    if any(part in IGNORE_PARTS for part in path.parts):
        return False
    return path.is_file() and path.suffix in {".c", ".h", ".hpp", ".cpp", ".txt", ".json", ".yml", ".yaml"}

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("symbol")
    args = ap.parse_args()

    found = 0
    for item in SCAN_DIRS:
        path = ROOT / item
        paths = [path] if path.is_file() else path.rglob("*")
        for p in paths:
            if not should_scan(p):
                continue
            for i, line in enumerate(p.read_text(errors="ignore").splitlines(), 1):
                if args.symbol in line:
                    print(f"{p.relative_to(ROOT)}:{i}:{line}")
                    found += 1

    print(f"\n{found} hit(s)")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
