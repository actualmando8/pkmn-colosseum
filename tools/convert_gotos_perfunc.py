#!/usr/bin/env python3
"""
Per-function goto converter with compile-check safety.
Applies goto_eliminator to each function individually,
verifies brace balance, and reverts if anything breaks.

Key insight: process ONE function at a time, re-scan after each to avoid
stale line number issues.
"""
import re
import sys
import os
import subprocess
import shutil

sys.path.insert(0, os.path.dirname(__file__))
from goto_eliminator import process_function, count_gotos


def find_functions_with_gotos(lines):
    """Find all functions that contain gotos, with correct line boundaries."""
    funcs = []
    depth = 0
    func_start = None
    for i, line in enumerate(lines):
        if depth == 0 and re.match(
            r'^(void|u32|s32|u16|s16|u8|s8|f32|f64|int|void\s*\*|static\s+void|static\s+u32|static\s+s32|static\s+int|\w+\s*\*)\s+\w+\s*\(',
            line
        ):
            func_start = i
        opens = line.count('{')
        closes = line.count('}')
        if depth == 0 and opens > 0 and func_start is None:
            func_start = i
        depth += opens - closes
        if depth == 0 and func_start is not None:
            func_lines = lines[func_start:i+1]
            gc = count_gotos(func_lines)
            if gc > 0:
                name_m = re.match(r'^(?:void|u32|s32|u16|s16|u8|s8|f32|f64|int|void\s*\*|static\s+\w+)\s+(\w+)', func_lines[0])
                name = name_m.group(1) if name_m else '???'
                funcs.append((func_start, i, name, gc))
            func_start = None
    return funcs


def compile_check(filepath):
    """Run compile check, return True if successful."""
    try:
        result = subprocess.run(
            [sys.executable, 'tools/compile_check.py', filepath],
            capture_output=True, text=True, timeout=120
        )
        return result.returncode == 0
    except Exception:
        return False


def process_file(filepath):
    total_eliminated = 0
    funcs_touched = 0
    failed_funcs = set()  # Track functions that fail compilation

    # Iterate: each pass processes ONE function, then re-scans
    for attempt in range(100):  # Safety limit
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            good_content = f.read()

        lines = good_content.split('\n')
        func_gotos = find_functions_with_gotos(lines)

        if not func_gotos:
            break

        made_progress = False

        for start, end, name, gc in func_gotos:
            if name in failed_funcs:
                continue

            func_lines = lines[start:end+1]
            new_func_lines, elim = process_function(list(func_lines))

            if elim == 0:
                failed_funcs.add(name)  # Mark as no-progress
                continue

            # Verify brace balance
            depth = 0
            for line in new_func_lines:
                depth += line.count('{') - line.count('}')
            if depth != 0:
                print(f'  {name}: SKIP - brace imbalance (depth={depth})')
                failed_funcs.add(name)
                continue

            # Apply conversion
            test_lines = list(lines)
            test_lines[start:end+1] = new_func_lines
            test_content = '\n'.join(test_lines)
            test_content = re.sub(r'\n{4,}', '\n\n\n', test_content)

            with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
                f.write(test_content)

            if compile_check(filepath):
                remain = gc - elim
                total_eliminated += elim
                funcs_touched += 1
                print(f'  {name}: -{elim}/{gc} ({remain} left) [OK]')
                made_progress = True
                break  # Re-scan from scratch with fresh line numbers
            else:
                print(f'  {name}: REVERT - compile failed')
                with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
                    f.write(good_content)
                failed_funcs.add(name)
                continue

        if not made_progress:
            break

    # Final count
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        final_content = f.read()
    final_gotos = count_gotos(final_content.split('\n'))
    print(f'\n  TOTAL: {total_eliminated} eliminated, {final_gotos} remaining ({funcs_touched} functions)')
    return total_eliminated


def main():
    if len(sys.argv) < 2:
        print("Usage: convert_gotos_perfunc.py <file.c>")
        sys.exit(1)

    filepath = sys.argv[1]
    if not os.path.exists(filepath):
        print(f'File not found: {filepath}')
        sys.exit(1)

    backup = filepath + '.perfunc.bak'
    shutil.copy2(filepath, backup)

    total = process_file(filepath)

    if total > 0:
        print(f'\nSuccess! {total} gotos eliminated.')
        if compile_check(filepath):
            print('Final compile: OK')
        else:
            print('Final compile: FAILED - reverting all changes')
            shutil.copy2(backup, filepath)
    else:
        print('\nNo gotos eliminated.')

    if os.path.exists(backup):
        os.remove(backup)


if __name__ == '__main__':
    main()
