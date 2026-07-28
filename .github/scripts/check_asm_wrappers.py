#!/usr/bin/env python3
"""Fail if any asm wrapper that pastes the target's own disassembly is live.

CONTRIBUTING forbids committing "asm-wrapper bodies as decompilation progress":
an `asm fn() { #include "fn.inc" }` reproduces the target byte for byte and
scores ~100% without being decompilation. The archived campaign had 370 of them.

quality_scan.py already blocks *newly added* `#include "*.inc"` lines in a PR
diff. It cannot see the other way one goes live: 1,487 of these wrappers sit in
the tree behind `#if 0` next to the real C, and flipping a `#if 0` to `#if 1`
(or deleting the `#else` branch) activates one while adding no `#include` line
at all. This walks the whole tree instead of the diff, so that route is covered.

Legitimate hand-written inline asm is untouched -- only blocks that pull in a
generated .inc count. At the time of writing there are 38 live asm blocks, all
genuine Dolphin SDK primitives (OSCache cache intrinsics, gbaCommunication,
OSTime), and zero live .inc wrappers.

usage: check_asm_wrappers.py [root]
"""
import os
import re
import sys

ASM_DECL = re.compile(r'^\s*asm\s+\w+[\w\s\*]*\w\s*\(')
IF_ZERO = re.compile(r'#\s*if\s+0\b')
IF_ANY = re.compile(r'#\s*if')
ELSE_ANY = re.compile(r'#\s*el(se|if)\b')
ENDIF = re.compile(r'#\s*endif\b')
INC = re.compile(r'#\s*include\s+"[^"]*\.inc"')

# How far past `asm foo() {` to look for the .inc include.
BODY_WINDOW = 12


def scan(path):
    """Return live asm blocks in `path` that include a generated .inc."""
    try:
        lines = open(path, errors='ignore').read().split('\n')
    except OSError:
        return []
    live = []
    # stack of bools: True where the enclosing conditional is a dead `#if 0`
    stack = []
    for i, line in enumerate(lines):
        s = line.strip()
        if IF_ZERO.match(s):
            stack.append(True)
        elif IF_ANY.match(s):
            stack.append(False)
        elif ELSE_ANY.match(s) and stack:
            stack[-1] = not stack[-1]
        elif ENDIF.match(s) and stack:
            stack.pop()
        elif ASM_DECL.match(s) and not any(stack):
            body = '\n'.join(lines[i:i + BODY_WINDOW])
            if INC.search(body):
                live.append((i + 1, s[:72]))
    return live


def main(argv):
    root = argv[1] if len(argv) > 1 else 'src'
    offenders = []
    scanned = 0
    for dirpath, _, files in os.walk(root):
        for fn in sorted(files):
            if not fn.endswith(('.c', '.cpp')):
                continue
            p = os.path.join(dirpath, fn)
            scanned += 1
            for lineno, text in scan(p):
                offenders.append((p, lineno, text))

    if offenders:
        print(f'FAIL: {len(offenders)} live asm wrapper(s) including a '
              f'generated .inc -- these reproduce the target rather than '
              f'decompile it:')
        for p, lineno, text in offenders:
            print(f'  {p}:{lineno}: {text}')
        print('\nKeep the wrapper behind `#if 0` beside the real C body, or '
              'delete it.')
        return 1
    print(f'OK: {scanned} source files scanned, no live .inc asm wrappers')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
