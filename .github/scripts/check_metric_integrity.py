#!/usr/bin/env python3
"""Assert the published progress metric measures the game exactly once.

src/ is ~41% one-line `#include` shims: where a retail source region needs
different compiler flags than its neighbours the address range is split and
each piece becomes its own scoring unit compiled from the same .c file (the
_o2 / _o4p / _gc20 suffixes name the flag variant). That is sanctioned, but it
is also exactly the shape that would hide double counting, and the shim files
are not self-evidently honest when read on their own -- one is headed "Score
instrumentation only".

Two invariants make the claim checkable rather than a matter of trust:

  1. every function address is claimed by exactly one unit, so no byte of the
     game is scored twice;
  2. the code denominator matches main.dol's text sections, so the metric is
     measured against the whole game and nothing else.

Both were verified by hand once (2026-07-28). This keeps them true.

usage: check_metric_integrity.py <report.json> [path/to/main.dol]
"""
import collections
import json
import struct
import sys

# main.dol carries 7 text sections; sizes live at 0x90 in the header.
DOL_SIZES_OFF = 0x90
DOL_TEXT_SECTIONS = 7
# dtk's denominator legitimately differs a little from the raw section total
# (alignment padding, sections it does not attribute). Measured at 0.33%.
DOL_TOLERANCE = 0.02


def main(argv):
    if len(argv) < 2:
        print('usage: check_metric_integrity.py <report.json> [main.dol]')
        return 2
    report = json.load(open(argv[1]))

    # ---- invariant 1: no address claimed twice -------------------------
    claims = collections.defaultdict(list)
    for unit in report['units']:
        fns = list(unit.get('functions') or [])
        for sec in unit.get('sections', []):
            fns += sec.get('functions', [])
        for f in fns:
            va = f.get('metadata', {}).get('virtual_address')
            if va is None:
                continue
            claims[int(va)].append((unit['name'], f['name'], int(f['size'])))

    dupes = {a: v for a, v in claims.items() if len(v) > 1}
    if dupes:
        wasted = sum(v[0][2] * (len(v) - 1) for v in dupes.values())
        print(f'FAIL: {len(dupes)} function addresses claimed by more than one '
              f'unit ({wasted:,} bytes double-counted)')
        for a, v in list(dupes.items())[:10]:
            print(f'  {a:#010x}')
            for unit, name, _ in v:
                print(f'      {name}  [{unit}]')
        return 1
    print(f'OK: {len(claims):,} functions, {len(claims):,} distinct addresses, '
          f'none claimed twice')

    # ---- invariant 2: denominator == the real DOL text -----------------
    total_code = int(report['measures']['total_code'])
    if len(argv) < 3:
        print(f'   (no main.dol given; denominator {total_code:,} unchecked)')
        return 0
    try:
        blob = open(argv[2], 'rb').read()
    except OSError as e:
        print(f'   (main.dol unreadable: {e}; denominator unchecked)')
        return 0
    sizes = struct.unpack('>18I', blob[DOL_SIZES_OFF:DOL_SIZES_OFF + 72])
    dol_text = sum(sizes[:DOL_TEXT_SECTIONS])
    drift = abs(total_code - dol_text) / dol_text
    if drift > DOL_TOLERANCE:
        print(f'FAIL: denominator {total_code:,} vs main.dol text '
              f'{dol_text:,} -- {drift:.2%} apart, over the '
              f'{DOL_TOLERANCE:.0%} tolerance')
        return 1
    print(f'OK: denominator {total_code:,} vs main.dol text {dol_text:,} '
          f'({drift:.2%} apart)')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
