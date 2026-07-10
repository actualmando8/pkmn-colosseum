#!/usr/bin/env python3
"""Ingest strike-session SOL_NOTES.jsonl into the KG's path_facts source.

Each meaningful note (non-empty facts) becomes one path_fact scoped to the
function's source file, carrying the score, status, facts, rejected
hypotheses, and next-exact-hypothesis. Dedupes by symbol (re-runs update in
place). Workers touching that file then inherit the strike's findings via
path-scoped injection.
"""
import json
import sys
from pathlib import Path

STRIKE_WTS = [
    Path("/Users/douglaswhittingham/sol-strike-wt"),
    Path("/Users/douglaswhittingham/strike55-wt"),
    Path("/Users/douglaswhittingham/spark-strike-wt"),
]
FACTS = Path(
    "/Users/douglaswhittingham/gamecube-decomp-harness/projects/pkmn-colosseum/"
    "knowledge/sources/injectable/path_facts/data/path_facts/game.jsonl"
)
TARGETS_TSV = Path("/Users/douglaswhittingham/pkmn-colosseum/.handoff/unattacked_all.tsv")


def load_target_map():
    m = {}
    for line in TARGETS_TSV.read_text().splitlines():
        parts = line.split("\t")
        if len(parts) >= 4:
            m[parts[0]] = parts[3]
    return m


def note_to_fact(note, src_path):
    sym = note["symbol"]
    score = note.get("final_fuzzy")
    status = note.get("status", "unknown")
    summary = [
        f"Strike attempt 2026-07-10: {sym} reached fuzzy {score} ({status}).",
    ]
    summary += [str(f) for f in note.get("facts", [])[:6]]
    do_not = [str(h) for h in note.get("rejected_hypotheses", [])[:4]]
    nxt = note.get("next_exact_hypothesis")
    do = ([f"Next exact hypothesis: {nxt}"] if nxt else []) + [
        "Start from the banked strike implementation in the function's TU (git history: strike/sol-large-band, strike/gpt55-large-tail, strike/spark-smalls worktrees) rather than from scratch.",
    ]
    return {
        "schema_version": "path_fact_v1",
        "id": f"path_fact:game:strike-note-{sym}",
        "kind": "path_fact",
        "directory": "game",
        "status": "accepted",
        "strength": "hint",
        "title": f"Strike findings for {sym} (fuzzy {score}, {status})",
        "scope_globs": [src_path],
        "applies_when": [f"target symbol is {sym}"],
        "summary": summary,
        "do": do,
        "do_not": do_not,
        "evidence_refs": ["strike SOL_NOTES.jsonl 2026-07-10, objdiff-scored"],
        "watched_paths": [src_path],
        "slice_ref": "",
        "superseded_by": ["report.json fuzzy>=100 for this symbol", "current asm evidence"],
        "curator_update_policy": {
            "target_source_id": "path_facts",
            "update_kind": "path_fact",
            "mutation_policy": "proposal_only_until_validated",
        },
    }


def main():
    tmap = load_target_map()
    existing = []
    seen_ids = set()
    for line in FACTS.read_text().splitlines():
        if not line.strip():
            continue
        rec = json.loads(line)
        existing.append(rec)
        seen_ids.add(rec["id"])

    added = updated = skipped = 0
    for wt in STRIKE_WTS:
        notes_file = wt / "SOL_NOTES.jsonl"
        if not notes_file.exists():
            continue
        for line in notes_file.read_text().splitlines():
            line = line.strip()
            if not line:
                continue
            try:
                note = json.loads(line)
            except json.JSONDecodeError:
                continue
            sym = note.get("symbol")
            if not sym or not note.get("facts"):
                skipped += 1
                continue
            src = tmap.get(sym)
            if not src:
                skipped += 1
                continue
            fact = note_to_fact(note, src)
            if fact["id"] in seen_ids:
                existing = [fact if r["id"] == fact["id"] else r for r in existing]
                updated += 1
            else:
                existing.append(fact)
                seen_ids.add(fact["id"])
                added += 1

    FACTS.write_text("\n".join(json.dumps(r) for r in existing) + "\n")
    print(f"path_facts: +{added} added, {updated} updated, {skipped} skipped; total {len(existing)}")


if __name__ == "__main__":
    sys.exit(main())
