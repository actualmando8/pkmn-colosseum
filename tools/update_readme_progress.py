#!/usr/bin/env python3
"""Update README progress metrics from build/GC6E01/report.json."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_README = ROOT / "README.md"
DEFAULT_REPORT = ROOT / "build" / "GC6E01" / "report.json"


def read_json(path: Path) -> dict[str, Any]:
    try:
        return json.loads(path.read_text())
    except FileNotFoundError:
        sys.exit(f"missing file: {path}")
    except json.JSONDecodeError as exc:
        sys.exit(f"invalid JSON in {path}: {exc}")


def as_int(value: Any, key: str) -> int:
    try:
        return int(value)
    except (TypeError, ValueError):
        raise ValueError(f"{key} is not an integer: {value!r}") from None


def as_float(value: Any, key: str) -> float:
    try:
        return float(value)
    except (TypeError, ValueError):
        raise ValueError(f"{key} is not a number: {value!r}") from None


def pct(value: Any, key: str) -> str:
    return f"{as_float(value, key):.2f}%"


def count(value: Any, key: str) -> str:
    return f"{as_int(value, key):,}"


def progress_table(report: dict[str, Any]) -> str:
    measures = report.get("measures", {})
    return "\n".join(
        [
            "| Metric | Value |",
            "|---|---|",
            f"| Fuzzy match | {pct(measures.get('fuzzy_match_percent'), 'fuzzy_match_percent')} |",
            (
                "| Function match | "
                f"{pct(measures.get('matched_functions_percent'), 'matched_functions_percent')} "
                f"({count(measures.get('matched_functions'), 'matched_functions')} / "
                f"{count(measures.get('total_functions'), 'total_functions')} functions) |"
            ),
            (
                "| Code match | "
                f"{pct(measures.get('matched_code_percent'), 'matched_code_percent')} "
                f"({count(measures.get('matched_code'), 'matched_code')} / "
                f"{count(measures.get('total_code'), 'total_code')} matched code bytes) |"
            ),
            (
                "| Data match | "
                f"{pct(measures.get('matched_data_percent'), 'matched_data_percent')} "
                f"({count(measures.get('matched_data'), 'matched_data')} / "
                f"{count(measures.get('total_data'), 'total_data')} matched data bytes) |"
            ),
        ]
    )


def update_readme(readme_text: str, table: str) -> str:
    pattern = re.compile(
        r"(?ms)^\| Metric \| Value \|\n"
        r"^\|---\|---\|\n"
        r"(?:^\|.*\|\n)+"
    )
    updated, replacements = pattern.subn(table + "\n", readme_text, count=1)
    if replacements != 1:
        sys.exit("could not find README status table")
    return updated


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Update README progress metrics from objdiff report.json."
    )
    parser.add_argument("--readme", type=Path, default=DEFAULT_README)
    parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    parser.add_argument(
        "--check",
        action="store_true",
        help="Fail if README is not already up to date",
    )
    args = parser.parse_args()

    report = read_json(args.report)
    readme_text = args.readme.read_text()
    updated = update_readme(readme_text, progress_table(report))

    if args.check:
        if updated != readme_text:
            sys.exit("README progress is stale; run tools/update_readme_progress.py")
        return 0

    args.readme.write_text(updated)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
