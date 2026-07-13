#!/usr/bin/env python3
"""Refresh a takeover target manifest from report and attempt state.

The input is the tab-separated target manifest accepted by the harness.  Exact
functions are always removed.  In ``unattacked`` mode, targets with a
substantive worker or manual attempt are removed as well, leaving only work
that has not yet received usable model output.
"""

from __future__ import annotations

import argparse
import csv
import json
import os
import sqlite3
import tempfile
from pathlib import Path
from typing import Iterable


MANIFEST_HEADER = ("target_key", "symbol", "size", "unit", "source", "band")
SUBSTANTIVE_WORKER_STATUSES = frozenset({"exact", "timeout"})
NON_SUBSTANTIVE_MANUAL_STATUSES = frozenset(
    {"error", "failed", "launch_failed", "provider_error"}
)


def exact_target_keys(report_path: Path) -> set[str]:
    report = json.loads(report_path.read_text())
    exact: set[str] = set()
    for unit in report.get("units", []):
        unit_name = unit.get("name")
        if not unit_name:
            continue
        for function in unit.get("functions", []):
            if function.get("fuzzy_match_percent") == 100.0 and function.get("name"):
                exact.add(f"{unit_name}::{function['name']}")
    return exact


def substantive_worker_keys(db_path: Path, run_ids: Iterable[str]) -> set[str]:
    ids = tuple(dict.fromkeys(run_ids))
    if not ids:
        return set()
    placeholders = ",".join("?" for _ in ids)
    statuses = tuple(sorted(SUBSTANTIVE_WORKER_STATUSES))
    status_placeholders = ",".join("?" for _ in statuses)
    query = f"""
        SELECT DISTINCT target_key
        FROM worker_state
        WHERE session_id IN ({placeholders})
          AND lifecycle_status IN ({status_placeholders})
    """
    with sqlite3.connect(db_path) as connection:
        return {
            str(row[0])
            for row in connection.execute(query, (*ids, *statuses))
            if row[0]
        }


def substantive_manual_keys(paths: Iterable[Path]) -> set[str]:
    keys: set[str] = set()
    for path in paths:
        if not path.exists():
            continue
        for line_number, line in enumerate(path.read_text().splitlines(), start=1):
            if not line.strip():
                continue
            try:
                record = json.loads(line)
            except json.JSONDecodeError as error:
                raise ValueError(f"{path}:{line_number}: invalid JSON: {error}") from error
            key = record.get("target_key")
            status = str(record.get("status", "attempted")).lower()
            if key and status not in NON_SUBSTANTIVE_MANUAL_STATUSES:
                keys.add(str(key))
    return keys


def read_manifest(path: Path) -> list[dict[str, str]]:
    with path.open(newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != MANIFEST_HEADER:
            raise ValueError(
                f"{path}: expected header {MANIFEST_HEADER}, got {reader.fieldnames}"
            )
        rows = list(reader)
    keys = [row["target_key"] for row in rows]
    if len(keys) != len(set(keys)):
        raise ValueError(f"{path}: duplicate target_key rows")
    return rows


def filter_manifest(
    rows: Iterable[dict[str, str]],
    *,
    exact: set[str],
    substantive: set[str],
    mode: str,
    excluded_sources: set[str],
) -> tuple[list[dict[str, str]], dict[str, int]]:
    kept: list[dict[str, str]] = []
    counts = {
        "input": 0,
        "exact_filtered": 0,
        "substantive_filtered": 0,
        "source_filtered": 0,
        "output": 0,
    }
    for row in rows:
        counts["input"] += 1
        key = row["target_key"]
        if key in exact:
            counts["exact_filtered"] += 1
        elif row["source"] in excluded_sources:
            counts["source_filtered"] += 1
        elif mode == "unattacked" and key in substantive:
            counts["substantive_filtered"] += 1
        else:
            kept.append(row)
    counts["output"] = len(kept)
    return kept, counts


def write_manifest(path: Path, rows: Iterable[dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, temporary_name = tempfile.mkstemp(prefix=f".{path.name}.", dir=path.parent)
    try:
        mode = path.stat().st_mode & 0o777 if path.exists() else 0o644
        os.fchmod(fd, mode)
        with os.fdopen(fd, "w", newline="") as stream:
            writer = csv.DictWriter(
                stream, fieldnames=MANIFEST_HEADER, delimiter="\t", lineterminator="\n"
            )
            writer.writeheader()
            writer.writerows(rows)
        os.replace(temporary_name, path)
    except Exception:
        try:
            os.unlink(temporary_name)
        except FileNotFoundError:
            pass
        raise


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--report", type=Path, required=True)
    parser.add_argument("--mode", choices=("nonexact", "unattacked"), default="unattacked")
    parser.add_argument("--db", type=Path)
    parser.add_argument("--run-id", action="append", default=[])
    parser.add_argument("--manual-attempts", type=Path, action="append", default=[])
    parser.add_argument("--exclude-source", action="append", default=[])
    args = parser.parse_args()
    if args.mode == "unattacked" and (args.db is None or not args.run_id):
        parser.error("unattacked mode requires --db and at least one --run-id")
    return args


def main() -> int:
    args = parse_args()
    exact = exact_target_keys(args.report)
    substantive = substantive_manual_keys(args.manual_attempts)
    if args.mode == "unattacked":
        substantive |= substantive_worker_keys(args.db, args.run_id)
    rows = read_manifest(args.input)
    kept, counts = filter_manifest(
        rows,
        exact=exact,
        substantive=substantive,
        mode=args.mode,
        excluded_sources=set(args.exclude_source),
    )
    write_manifest(args.output, kept)
    print(json.dumps(counts, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
