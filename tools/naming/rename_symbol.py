#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]

SYMBOLS_FILE = REPO_ROOT / "config" / "GC6E01" / "symbols.txt"

ACTIVE_DIRS = [
    REPO_ROOT / "src",
    REPO_ROOT / "include",
]

IGNORE_PARTS = {
    "archive",
    "build",
    ".git",
    "__pycache__",
}

TEXT_EXTS = {
    ".c", ".h", ".hpp", ".cpp", ".s", ".S", ".txt", ".md", ".json", ".yml", ".yaml",
}


def should_scan(path: Path) -> bool:
    if any(part in IGNORE_PARTS for part in path.parts):
        return False
    if path.suffix not in TEXT_EXTS:
        return False
    return path.is_file()


def replace_identifier(text: str, old: str, new: str) -> tuple[str, int]:
    pattern = re.compile(rf"\b{re.escape(old)}\b")
    return pattern.subn(new, text)


def main() -> int:
    parser = argparse.ArgumentParser(description="Safely rename a symbol in active project files.")
    parser.add_argument("old", help="Old symbol name, e.g. fn_801AA6D0")
    parser.add_argument("new", help="New symbol name, e.g. PObjAmnesia")
    parser.add_argument("--dry-run", action="store_true", help="Show replacements without writing files")
    parser.add_argument("--symbols-only", action="store_true", help="Only update symbols.txt")
    args = parser.parse_args()

    old = args.old
    new = args.new

    if old == new:
        raise SystemExit("old and new names are identical")

    changed: list[tuple[Path, int]] = []

    # symbols.txt must contain old symbol and must not already contain new symbol definition.
    symbols = SYMBOLS_FILE.read_text()
    old_marker = f"{old} = "
    new_marker = f"{new} = "

    if old_marker not in symbols:
        raise SystemExit(f"ERROR: {old!r} was not found as a symbol definition in {SYMBOLS_FILE}")

    if new_marker in symbols:
        raise SystemExit(f"ERROR: {new!r} already exists as a symbol definition in {SYMBOLS_FILE}")

    new_symbols = symbols.replace(old_marker, new_marker, 1)
    changed.append((SYMBOLS_FILE, 1))

    if not args.dry_run:
        SYMBOLS_FILE.write_text(new_symbols)

    if not args.symbols_only:
        for root in ACTIVE_DIRS:
            if not root.exists():
                continue
            for path in root.rglob("*"):
                if not should_scan(path):
                    continue

                text = path.read_text(errors="ignore")
                updated, count = replace_identifier(text, old, new)

                if count:
                    changed.append((path, count))
                    if not args.dry_run:
                        path.write_text(updated)

    action = "would update" if args.dry_run else "updated"
    print(f"{action} {len(changed)} file(s):")
    for path, count in changed:
        print(f"  {path.relative_to(REPO_ROOT)}: {count} replacement(s)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
