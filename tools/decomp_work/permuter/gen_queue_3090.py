#!/usr/bin/env python3
"""gen_queue_3090.py — regenerate build/permuter_queue_3090.tsv for the 3090 farm.

Pool = every function in build/GC6E01/report.json with fuzzy_match_percent in
[75, 100) on a real (non-auto-generated) source unit.
  tier 1 : 90.000-99.99x  (near-misses — permuter prime targets)
  tier 2 : 75.000-89.99x  (floor lowered 80->75 on 2026-07-02 to admit the
                           menu_range 77% hoisting-wall long shots)
Sorted tier asc, then pct desc (so 99.9%+ land at the very top), then size asc.

Columns (same as the original): tier, pct, size, name, addr, unit.
Writes the TSV and prints a diff summary vs the previous queue file.
"""
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
REPORT = os.path.join(ROOT, "build", "GC6E01", "report.json")
OUT = os.path.join(ROOT, "build", "permuter_queue_3090.tsv")

# ---- permanent de-queues (wall classes NOT winnable by source permutation) --
# gs_title SDA2 anonymous-constant pairing blocker: needs a symbols.txt fix,
# not source mutation (2026-07-02, wave-3 triage).
DENY_FNS = {
    "fn_8002520C", "fn_80024DBC", "fn_8002509C", "fn_80024F2C", "fn_8002537C",
}

# Reserved for the separately coordinated fight-range campaign.
DENY_UNITS = {
    "main/game/fight_range_80211A00",
}


def denied(unit, name, pct):
    if unit in DENY_UNITS:
        return True
    if name in DENY_FNS:
        return True
    # colosseum_battle position-dependent codegen wall: every member sits at
    # exactly 87.78% — identical source compiles differently by file position.
    if unit.endswith("colosseum_battle") and abs(pct - 87.78) < 0.005:
        return True
    # MusyX synthmacros mcmd setter family (13 members @99.61539 exactly, the
    # ex-people_field PF_DEFINE_MOTION_SETTER wrappers): proven source-unfixable
    # -- even the MusyX reference's own shape yields the residual (wave-4 triage).
    if unit.endswith("musyx/runtime/synthmacros") and abs(pct - 99.61539) < 0.001:
        return True
    return False


def priority(unit, name, pct):
    """Sort band within a tier (lower = earlier). Coordinator-directed:
    0: macHandleActive (fn_801557EC) — biggest single payoff in the queue
    1: fight_range_80201764 pinned family (cosmetic prologue residue)
    2: MusyX units (wave-4/5 prime targets)
    3: everything else
    4: colosseum_battle — PARKED, not removed: the other live session is
       actively closing that lane by hand (2026-07-02 verdict: the ~55
       re-entered WazaHit near-misses are its transient work, not a
       regression). Whatever it leaves parked becomes farm food later.
    """
    if name == "fn_801557EC":
        return 0
    if unit.endswith("fight_range_80201764") and pct >= 99.5:
        return 1
    if "/musyx/" in unit or unit.endswith("musyx_range_801652DC"):
        return 2
    if unit.endswith("colosseum_battle"):
        return 4
    return 3


def main():
    report = json.load(open(REPORT))
    rows = []
    for u in report["units"]:
        meta = u.get("metadata") or {}
        if meta.get("auto_generated"):
            continue
        for fn in u.get("functions", []):
            pct = fn.get("fuzzy_match_percent")
            if pct is None or not (75.0 <= pct < 100.0):
                continue
            if denied(u["name"], fn["name"], pct):
                continue
            va = int((fn.get("metadata") or {}).get("virtual_address", "0"))
            tier = 1 if pct >= 90.0 else 2
            rows.append((tier, pct, int(fn["size"]), fn["name"],
                         f"0x{va:08X}", u["name"]))
    rows.sort(key=lambda r: (r[0], priority(r[5], r[3], r[1]), -r[1], r[2]))

    old = set()
    if os.path.exists(OUT):
        for ln in open(OUT):
            if ln.startswith("#") or not ln.strip():
                continue
            old.add(ln.rstrip("\n").split("\t")[3])
    new = {r[3] for r in rows}

    with open(OUT, "w", encoding="utf-8") as f:
        f.write("# tier\tpct\tsize\tname\taddr\tunit\n")
        for t, pct, size, name, addr, unit in rows:
            f.write(f"{t}\t{pct:.3f}\t{size}\t{name}\t{addr}\t{unit}\n")

    t1 = sum(1 for r in rows if r[0] == 1)
    print(f"wrote {OUT}: {len(rows)} entries (tier1={t1}, tier2={len(rows)-t1})")
    retired = sorted(old - new)
    added = sorted(new - old)
    print(f"retired ({len(retired)}): {', '.join(retired)}")
    print(f"added ({len(added)}): {', '.join(added)}")


if __name__ == "__main__":
    main()
