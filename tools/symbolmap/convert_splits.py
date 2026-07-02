#!/usr/bin/env python3
"""convert_splits.py - adapt config/GC6E01/splits.txt to the old
splits_refined.txt row format that attribute_tus.py (unchanged, ported
verbatim from the archived campaign) expects.

Why this exists
----------------
The archived pipeline's attribute_tus.py reads a three-state
`KNOWN|LIKELY|GAP  0xSTART  0xEND  src` row format
(config/GC6E01/splits_refined.txt in the old tree). The live tree's build
uses decomp-toolkit's own splits.txt format instead:

    some/unit/path.c:
        .text    start:0xADDR end:0xADDR
        .rodata  start:0xADDR end:0xADDR
        ...

There is no explicit KNOWN/GAP marker in this format, but the *unit name*
still carries the same information the old GAP status carried: units that
dtk could not attribute to a real source file are named generically by dtk
itself, embedding their own start address, e.g. `game/gs_range_800D1070.c`
or `dolphin/si/SI_fn_800CF708.c`. Units with a real, hand-attributed name
(`game/gs_render_util.c`, `crt/string.c`, ...) carry no such embedded
address. We use exactly that distinction to synthesize KNOWN/GAP:

    GAP    unit basename ends in `_range_XXXXXXXX.c(pp)` or `_fn_XXXXXXXX.c(pp)`
    KNOWN  everything else

Only `.text`/`.init` ranges are emitted - functions never live in
.rodata/.data/.sdata/.sdata2/.bss, and attribute_tus.py's range lookup is
keyed off function addresses only, so including data ranges would just be
inert noise (and DATA_SECTIONS ranges can be address-adjacent to code
ranges in a way that isn't guaranteed not to overlap across section kinds
on some layouts - simplest and safest to just leave them out).

This script performs *no evidence judgement* of its own - it is pure
plumbing so the unmodified attribute_tus.py can run against the current
splits.txt. All GAP/KNOWN/relabel/split classification still happens in
attribute_tus.py exactly as archived.

Usage:
    python tools/symbolmap/convert_splits.py \
        --splits config/GC6E01/splits.txt \
        --out config/GC6E01/symbolmap/splits_compat.txt
"""

import argparse
import re
from pathlib import Path

UNIT_HEADER = re.compile(r"^(?P<src>\S+\.(?:c|cpp)):\s*$")
SECTION_ROW = re.compile(
    r"^\s*\.(?P<kind>\w+)\s+start:0x(?P<start>[0-9A-Fa-f]+)\s+"
    r"end:0x(?P<end>[0-9A-Fa-f]+)\s*$")
CODE_KINDS = ("text", "init")

# dtk's own auto-naming for units it could not attribute to real source:
# `<prefix>_range_ADDR.c` (a run of GAP functions) or `<prefix>_fn_ADDR.c`
# (a single carved-out GAP function). Both embed their own start address.
GAP_NAME = re.compile(r"_(?:range|fn)_[0-9A-Fa-f]{8}\.(?:c|cpp)$")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--splits", required=True, type=Path,
                    help="config/GC6E01/splits.txt (dtk-template format)")
    ap.add_argument("--out", required=True, type=Path,
                    help="output path, old KNOWN/GAP row format")
    args = ap.parse_args()

    rows = []
    src = None
    for line in args.splits.read_text(encoding="utf-8").splitlines():
        m_hdr = UNIT_HEADER.match(line)
        if m_hdr:
            src = m_hdr.group("src")
            continue
        m_row = SECTION_ROW.match(line)
        if m_row and src and m_row.group("kind") in CODE_KINDS:
            status = "GAP" if GAP_NAME.search(src) else "KNOWN"
            rows.append((status, int(m_row.group("start"), 16),
                        int(m_row.group("end"), 16), src))

    rows.sort(key=lambda r: r[1])
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("w", encoding="utf-8") as f:
        for status, start, end, s in rows:
            f.write(f"{status} 0x{start:X} 0x{end:X} {s}\n")

    known = sum(1 for r in rows if r[0] == "KNOWN")
    gap = sum(1 for r in rows if r[0] == "GAP")
    print(f"[splits] {len(rows)} code ranges from {args.splits} "
          f"(KNOWN={known} GAP={gap})")
    print(f"[splits] wrote {args.out}")


if __name__ == "__main__":
    main()
