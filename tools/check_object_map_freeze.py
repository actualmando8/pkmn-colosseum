#!/usr/bin/env python3
"""Check that the GC6E01 dtk report denominator and unit topology stayed frozen."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_REPORT = ROOT / "build" / "GC6E01" / "report.json"
DEFAULT_FREEZE = ROOT / "config" / "GC6E01" / "object_map.freeze.json"

INVARIANT_KEYS = ("total_code", "total_data", "total_functions", "total_units")
PROGRESS_KEYS = (
    "matched_code",
    "matched_code_percent",
    "matched_data",
    "matched_data_percent",
    "matched_functions",
    "matched_functions_percent",
    "fuzzy_match_percent",
)
TOPOLOGY_STATUS_KEYS = (
    "auto_units",
    "named_units",
    "source_backed_units",
    "auto_code",
    "auto_data",
    "auto_functions",
)


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


def numeric_measure(measures: dict[str, Any], key: str, default: int = 0) -> int:
    return as_int(measures.get(key, default), key)


def section_signature(section: dict[str, Any]) -> dict[str, Any]:
    metadata = section.get("metadata", {})
    virtual_address = metadata.get("virtual_address")
    return {
        "name": section.get("name", ""),
        "size": as_int(section.get("size", 0), "section.size"),
        "virtual_address": (
            as_int(virtual_address, "section.metadata.virtual_address")
            if virtual_address is not None
            else None
        ),
    }


def unit_signature(unit: dict[str, Any]) -> dict[str, Any]:
    measures = unit.get("measures", {})
    return {
        "name": unit.get("name", ""),
        "sections": [section_signature(section) for section in unit.get("sections", [])],
        "total_code": numeric_measure(measures, "total_code"),
        "total_data": numeric_measure(measures, "total_data"),
        "total_functions": numeric_measure(measures, "total_functions"),
    }


def summarize_report(report: dict[str, Any], target: str) -> dict[str, Any]:
    measures = report.get("measures", {})
    units = report.get("units", [])
    auto_units = [
        unit for unit in units if unit.get("metadata", {}).get("auto_generated", False)
    ]

    invariants = {
        key: numeric_measure(measures, key)
        for key in INVARIANT_KEYS
    }
    topology_status = {
        "auto_units": len(auto_units),
        "named_units": len(units) - len(auto_units),
        "source_backed_units": sum(
            1 for unit in units if unit.get("metadata", {}).get("source_path")
        ),
        "auto_code": sum(
            numeric_measure(unit.get("measures", {}), "total_code") for unit in auto_units
        ),
        "auto_data": sum(
            numeric_measure(unit.get("measures", {}), "total_data") for unit in auto_units
        ),
        "auto_functions": sum(
            numeric_measure(unit.get("measures", {}), "total_functions")
            for unit in auto_units
        ),
    }
    baseline_progress = {key: measures.get(key) for key in PROGRESS_KEYS if key in measures}

    return {
        "schema": 1,
        "target": target,
        "purpose": (
            "Freeze the dtk/objdiff denominator and object topology used for "
            "published GC6E01 progress. Progress counters may increase without "
            "updating this file."
        ),
        "invariants": invariants,
        "topology_status": topology_status,
        "baseline_progress": baseline_progress,
        "unit_topology": [unit_signature(unit) for unit in units],
    }


def find_topology_drift(
    expected: list[dict[str, Any]],
    actual: list[dict[str, Any]],
) -> list[str]:
    errors: list[str] = []
    if len(expected) != len(actual):
        errors.append(f"unit count changed: expected {len(expected)}, got {len(actual)}")

    expected_names = [unit["name"] for unit in expected]
    actual_names = [unit["name"] for unit in actual]
    missing = sorted(set(expected_names) - set(actual_names))
    added = sorted(set(actual_names) - set(expected_names))
    if missing:
        errors.append("missing units: " + ", ".join(missing[:10]))
    if added:
        errors.append("added units: " + ", ".join(added[:10]))

    for index, (expected_unit, actual_unit) in enumerate(zip(expected, actual)):
        if expected_unit != actual_unit:
            errors.append(
                "first topology mismatch at "
                f"#{index}: expected {expected_unit.get('name')}, "
                f"got {actual_unit.get('name')}"
            )
            break

    return errors


def compare_freeze(
    expected: dict[str, Any],
    actual: dict[str, Any],
    strict_source_status: bool,
) -> tuple[list[str], list[str]]:
    errors: list[str] = []
    notes: list[str] = []

    if expected.get("target") != actual.get("target"):
        errors.append(
            f"target changed: expected {expected.get('target')}, got {actual.get('target')}"
        )

    expected_invariants = expected.get("invariants", {})
    actual_invariants = actual.get("invariants", {})
    for key in INVARIANT_KEYS:
        if expected_invariants.get(key) != actual_invariants.get(key):
            errors.append(
                f"{key} changed: expected {expected_invariants.get(key)}, "
                f"got {actual_invariants.get(key)}"
            )

    errors.extend(
        find_topology_drift(
            expected.get("unit_topology", []),
            actual.get("unit_topology", []),
        )
    )

    expected_topology = expected.get(
        "topology_status", expected.get("source_status", {})
    )
    actual_topology = actual.get("topology_status", {})
    topology_drift = [
        f"{key}: expected {expected_topology.get(key)}, got {actual_topology.get(key)}"
        for key in TOPOLOGY_STATUS_KEYS
        if expected_topology.get(key) != actual_topology.get(key)
    ]
    if topology_drift:
        message = "named/auto status changed; topology is the real freeze: " + "; ".join(
            topology_drift
        )
        if strict_source_status:
            errors.append(message)
        else:
            notes.append(message)

    return errors, notes


def write_freeze(path: Path, summary: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(summary, indent=2) + "\n")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Validate the frozen GC6E01 object-map denominator."
    )
    parser.add_argument(
        "--report",
        type=Path,
        default=DEFAULT_REPORT,
        help="dtk/objdiff report to inspect",
    )
    parser.add_argument(
        "--freeze",
        type=Path,
        default=DEFAULT_FREEZE,
        help="tracked freeze manifest",
    )
    parser.add_argument(
        "--target",
        default="GC6E01",
        help="target version label stored in the freeze manifest",
    )
    parser.add_argument(
        "--update",
        action="store_true",
        help="rewrite the freeze manifest from the current report",
    )
    parser.add_argument(
        "--strict-source-status",
        action="store_true",
        help="also fail when units move between auto-generated and source-backed",
    )
    args = parser.parse_args()

    report = read_json(args.report)
    summary = summarize_report(report, args.target)

    if args.update:
        write_freeze(args.freeze, summary)
        print(f"wrote {args.freeze}")
        return 0

    expected = read_json(args.freeze)
    errors, notes = compare_freeze(expected, summary, args.strict_source_status)
    for note in notes:
        print(f"note: {note}")

    invariants = summary["invariants"]
    if errors:
        print("object map freeze FAILED:", file=sys.stderr)
        for error in errors:
            print(f"- {error}", file=sys.stderr)
        print(
            "If this was an intentional split/topology change, rebuild the report "
            "and run tools/check_object_map_freeze.py --update in the same change.",
            file=sys.stderr,
        )
        return 1

    print(
        "object map freeze OK: "
        f"{invariants['total_units']} units, "
        f"{invariants['total_functions']} functions, "
        f"{invariants['total_code']} code bytes, "
        f"{invariants['total_data']} data bytes"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
