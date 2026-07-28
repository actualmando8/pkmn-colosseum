#!/usr/bin/env python3
"""Find memory accesses that read a different struct field than retail does.

This looks for *correctness* bugs, not score. When our source names the wrong
struct member the diff shows a load at the wrong displacement -- and because a
single instruction is worth almost nothing, it never surfaces as a scoring
target. `fn_80044378` tested `sprite->messageId` (s32 at +0x4c) where retail
reads `lha 0x6` (s16 eventId at +6); 0x12B2 is an eventId value and appears as
a case label in the switch three lines below. Fixing it moved the score by
0.004pp, so nothing that ranks by gap would ever have found it.

Signature: at an index-aligned position both sides are a displacement-form
memory op, the displacements or the access widths differ, and the surrounding
instructions still match -- so the two really are the same access site.

Every hit needs checking by hand. Most are NOT bugs -- on the first run over
the repo, three of the four highest-ranked hits were addressing differences
that compute the same effective address:

  * different base pointer. `__DSPHandler` reads `0x500a(r3)` where retail
    reads `0xa(r3)`; the DSP registers live at 0xCC005000, so retail's base
    already includes the 0x5000 and both reach the same word. Same for
    `fn_8009CE8C`'s 0x680c (SI registers at 0xCC006800).
  * pointer-advance vs indexed access. `fn_802249B8` emits `lbzu r0, 0x3(r31)`
    then reads `0x0(r31)` at five later sites, where retail keeps the base and
    reads `0x3(r31)` each time. Same address, different source shape.
  * a swapped switch arm. `fn_8004A7A8`'s `0x34` against retail's `0x30` was
    two arms in the wrong order, not a wrong member.

The real find that motivated this tool was `fn_80044378`, where the base
register and context matched and only the member was wrong. So: check that the
base register holds the same thing before concluding anything, and confirm
against the target asm.

usage: find_field_mismatch.py [min_align] [min_size]
"""
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
os.chdir(ROOT)
MIN_ALIGN = float(sys.argv[1]) if len(sys.argv) > 1 else 60.0
MIN_SIZE = int(sys.argv[2]) if len(sys.argv) > 2 else 200

# displacement-form memory ops: mnemonic rD, DISP(rA)
MEM = re.compile(r'^(l|st)(bz|hz|ha|wz|wzu|fs|fd|mw|w)u?\s+([rf]\d+),\s*'
                 r'(-?0x[0-9a-fA-F]+|-?\d+)\((r\d+)\)$')
WIDTH = {'bz': 1, 'hz': 2, 'ha': 2, 'wz': 4, 'w': 4, 'fs': 4, 'fd': 8}

report = json.load(open('build/GC6E01/report.json'))
units = []
for u in report['units']:
    fns = list(u.get('functions', []))
    for s in u.get('sections', []):
        fns += s.get('functions', [])
    sel = [f['name'] for f in fns
           if f.get('fuzzy_match_percent', 0.0) < 99.5
           and int(f['size']) >= MIN_SIZE]
    if sel:
        units.append((u['name'], sel))


def text(ins):
    return ins.get('instruction', {}).get('formatted', '') if ins else ''


def scan(item):
    unit, sel = item
    try:
        out = subprocess.run(
            ['build/tools/objdiff-cli', 'diff', '-p', '.', '-u', unit,
             '-o', '-', '--format', 'json'],
            capture_output=True, text=True, timeout=120).stdout
        d = json.loads(out)
        idx = {s: {x['name']: x for x in d[s]['symbols']}
               for s in ('left', 'right')}
    except Exception:
        return []
    rows = []
    for fn in sel:
        left, right = idx['left'].get(fn), idx['right'].get(fn)
        if not left or not right:
            continue
        L, R = left['instructions'], right['instructions']
        if len(L) != len(R):
            continue
        aligned = sum(1 for i in R if 'diff_kind' not in i)
        align = 100.0 * aligned / max(len(R), 1)
        if align < MIN_ALIGN:
            continue
        for i in range(len(L)):
            ml, mr = MEM.match(text(L[i])), MEM.match(text(R[i]))
            if not ml or not mr:
                continue
            same_disp = ml.group(4) == mr.group(4)
            same_width = WIDTH.get(ml.group(2)) == WIDTH.get(mr.group(2))
            same_kind = ml.group(1) == mr.group(1)      # load vs store
            if same_disp and same_width:
                continue
            if not same_kind:
                continue          # load-vs-store is a different problem
            # r1-relative accesses are frame layout, not struct fields; those
            # belong to find_frame_mismatch.py and are pure noise here
            if ml.group(5) == 'r1' or mr.group(5) == 'r1':
                continue
            # require local context to still line up, so this really is the
            # same access site rather than two unrelated instructions that
            # happened to land on the same index
            ctx = sum(1 for j in (i - 2, i - 1, i + 1, i + 2)
                      if 0 <= j < len(L) and text(L[j]) == text(R[j])
                      and text(L[j]))
            if ctx < 2:
                continue
            rows.append((align, fn, unit, i, text(L[i]), text(R[i])))
    return rows


rows = []
with ThreadPoolExecutor(max_workers=8) as ex:
    for r in ex.map(scan, units):
        rows += r
rows.sort(key=lambda r: -r[0])

print(f'{len(rows)} same-site memory accesses differing in offset or width')
print('Each needs checking against the struct by hand: a differing '
      'displacement\ncan also mean swapped switch arms rather than a wrong '
      'field.\n')
seen = set()
for align, fn, unit, i, lt, rt in rows:
    if fn in seen:
        continue
    seen.add(fn)
    print(f'{align:5.1f}% aligned  {fn}')
    print(f'          retail: {lt}')
    print(f'          ours:   {rt}')
    print(f'          [{unit}]')
