#!/usr/bin/env python3

import json
import sqlite3
import tempfile
import unittest
from pathlib import Path

from refresh_takeover_manifest import (
    MANIFEST_HEADER,
    exact_target_keys,
    filter_manifest,
    read_manifest,
    substantive_manual_keys,
    substantive_worker_keys,
    write_manifest,
)


class RefreshTakeoverManifestTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.root = Path(self.temporary.name)

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def test_refreshes_to_nonexact_unattempted_targets(self) -> None:
        report = self.root / "report.json"
        report.write_text(
            json.dumps(
                {
                    "units": [
                        {
                            "name": "main/unit",
                            "functions": [
                                {"name": "exact", "fuzzy_match_percent": 100.0},
                                {"name": "partial", "fuzzy_match_percent": 85.0},
                            ],
                        }
                    ]
                }
            )
        )
        rows = [
            self.row("exact"),
            self.row("partial"),
            self.row("timed_out"),
            self.row("launch_failed"),
            self.row("manual"),
            self.row("missing"),
            self.row("fight", source="src/game/fight_range_80211A00.c"),
        ]

        db = self.root / "state.sqlite"
        with sqlite3.connect(db) as connection:
            connection.execute(
                "CREATE TABLE worker_state "
                "(session_id TEXT, target_key TEXT, lifecycle_status TEXT)"
            )
            connection.executemany(
                "INSERT INTO worker_state VALUES (?, ?, ?)",
                [
                    ("run", "main/unit::timed_out", "timeout"),
                    ("run", "main/unit::launch_failed", "error"),
                    ("other", "main/unit::missing", "timeout"),
                ],
            )
        manual = self.root / "manual.jsonl"
        manual.write_text(
            json.dumps({"target_key": "main/unit::manual", "status": "improved"})
            + "\n"
        )

        substantive = substantive_worker_keys(db, ["run"])
        substantive |= substantive_manual_keys([manual])
        kept, counts = filter_manifest(
            rows,
            exact=exact_target_keys(report),
            substantive=substantive,
            mode="unattacked",
            excluded_sources={"src/game/fight_range_80211A00.c"},
        )

        self.assertEqual(
            [row["target_key"] for row in kept],
            ["main/unit::partial", "main/unit::launch_failed", "main/unit::missing"],
        )
        self.assertEqual(
            counts,
            {
                "input": 7,
                "exact_filtered": 1,
                "substantive_filtered": 2,
                "source_filtered": 1,
                "output": 3,
            },
        )

    def test_manifest_round_trip_and_duplicate_guard(self) -> None:
        path = self.root / "targets.tsv"
        rows = [self.row("one"), self.row("two")]
        write_manifest(path, rows)
        self.assertEqual(read_manifest(path), rows)
        self.assertEqual(path.stat().st_mode & 0o777, 0o644)

        write_manifest(path, [rows[0], rows[0]])
        with self.assertRaisesRegex(ValueError, "duplicate target_key"):
            read_manifest(path)

    @staticmethod
    def row(symbol: str, source: str = "src/unit.c") -> dict[str, str]:
        return dict(
            zip(
                MANIFEST_HEADER,
                (f"main/unit::{symbol}", symbol, "100", "main/unit", source, "small"),
            )
        )


if __name__ == "__main__":
    unittest.main()
