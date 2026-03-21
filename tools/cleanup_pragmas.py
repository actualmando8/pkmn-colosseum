#!/usr/bin/env python3
"""
cleanup_pragmas.py - Remove remaining non-essential pragma wrappers:
  - #pragma push / #pragma force_active on / #pragma pop (around non-asm blocks)
  - #pragma peephole off / #pragma peephole on
  - #pragma dont_inline off / #pragma dont_inline on
  - Orphaned #pragma push / #pragma pop without optimization_level (non-asm)
  - #pragma section directives
  - Stray #pragma weak (trk/udp_cc.c)

Does NOT touch:
  - #pragma push/pop/optimization_level 0/optimizewithasm off wrapping asm functions
"""

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"


def has_asm_in_block(lines, start, end):
    """Check if a push/pop block contains asm functions."""
    for i in range(start, end + 1):
        s = lines[i].strip()
        if s.startswith('asm ') or ' asm ' in s or 'nofralloc' in s or '__asm' in s:
            return True
    return False


def process_file(filepath):
    """Remove non-essential pragmas from a file."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()
    lines = [l.rstrip('\n').rstrip('\r') for l in lines]

    removals = set()
    changed = False

    # Pass 1: Find and remove simple pragma lines
    for i, line in enumerate(lines):
        s = line.strip()
        if s in ('#pragma peephole off', '#pragma peephole on',
                 '#pragma dont_inline off', '#pragma dont_inline on'):
            removals.add(i)

    # Pass 2: Find #pragma push / #pragma pop blocks that only contain force_active
    # (no optimization_level 0 inside -- those with opt_level are asm blocks)
    i = 0
    while i < len(lines):
        s = lines[i].strip()
        if s == '#pragma push':
            start = i
            depth = 1
            has_opt_level = False
            has_force_active = False
            j = i + 1
            while j < len(lines) and depth > 0:
                sj = lines[j].strip()
                if sj == '#pragma push':
                    depth += 1
                elif sj == '#pragma pop':
                    depth -= 1
                elif sj == '#pragma optimization_level 0':
                    has_opt_level = True
                elif sj == '#pragma force_active on':
                    has_force_active = True
                j += 1
            end = j - 1

            if has_force_active and not has_opt_level:
                # This is a pure force_active wrapper - remove push, force_active, pop
                removals.add(start)
                # Remove all pragma lines in the block (but keep function code)
                for k in range(start, end + 1):
                    sk = lines[k].strip()
                    if sk in ('#pragma push', '#pragma pop', '#pragma force_active on'):
                        removals.add(k)
            i = end + 1
        else:
            i += 1

    if removals:
        new_lines = [lines[i] for i in range(len(lines)) if i not in removals]
        with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
            for line in new_lines:
                f.write(line + '\n')
        changed = True

    return changed, len(removals)


def main():
    src_files = sorted(SRC_DIR.rglob("*.c"))
    total_removed = 0
    files_cleaned = 0

    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        if '#pragma' not in content:
            continue

        # Skip files that only have asm pragmas
        has_non_asm_pragma = False
        for pat in ['#pragma force_active', '#pragma peephole', '#pragma dont_inline']:
            if pat in content:
                has_non_asm_pragma = True
                break

        if not has_non_asm_pragma:
            continue

        rel = src_file.relative_to(PROJECT_ROOT)
        changed, count = process_file(str(src_file))
        if changed:
            files_cleaned += 1
            total_removed += count
            print(f"  {rel}: removed {count} pragma lines")

    print(f"\nCleaned {files_cleaned} files, removed {total_removed} pragma lines")

    # Final pragma count
    total_pragmas = 0
    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            for line in f:
                if '#pragma' in line.strip() and not line.strip().startswith('/*') and not line.strip().startswith('*'):
                    total_pragmas += 1
    print(f"Total remaining #pragma lines: {total_pragmas}")


if __name__ == '__main__':
    sys.exit(main() or 0)
