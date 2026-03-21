#!/usr/bin/env python3
"""
fix_call_mismatch.py - Fix 'function call does not match' errors in pragma asm functions.

For calls like `FunctionName()` in #pragma optimizewithasm off functions where
the function has been declared with parameters, replace with:
  ((void(*)(void))FunctionName)()
"""

import re
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
COMPILE_CMD = [sys.executable, str(PROJECT_ROOT / "tools" / "compile_check.py")]


def compile_file(src_file):
    r = subprocess.run(
        COMPILE_CMD + [str(src_file)],
        capture_output=True, text=True, cwd=str(PROJECT_ROOT)
    )
    return r.returncode == 0, (r.stdout or '') + '\n' + (r.stderr or '')


def parse_call_mismatch_errors(output):
    """Return list of (line_no, func_name) for call mismatch errors."""
    results = []
    lines = output.split('\n')
    current_line = None
    for raw_line in lines:
        m = re.match(r'#\s+(\d+):', raw_line)
        if m:
            current_line = int(m.group(1))
        m = re.match(r"#\s+function call '([^']+)\(\)' does not match", raw_line)
        if m and current_line:
            results.append((current_line, m.group(1)))
    return results


def fix_file(src_file):
    """Fix call mismatch errors. Returns True if modified."""
    success, output = compile_file(src_file)
    if success:
        return False

    mismatches = parse_call_mismatch_errors(output)
    if not mismatches:
        return False

    with open(src_file, 'r') as f:
        source_lines = f.readlines()

    modified = False
    for line_no, func_name in mismatches:
        idx = line_no - 1
        if idx < 0 or idx >= len(source_lines):
            continue

        line = source_lines[idx]
        # Replace FuncName() with ((void(*)(void))FuncName)()
        old_call = f"{func_name}()"
        new_call = f"((void(*)(void)){func_name})()"

        if old_call in line and new_call not in line:
            source_lines[idx] = line.replace(old_call, new_call)
            modified = True

    if modified:
        with open(src_file, 'w') as f:
            f.writelines(source_lines)

    return modified


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/fix_call_mismatch.py <source_file>")
        return

    src_file = Path(sys.argv[1])
    if not src_file.is_absolute():
        src_file = PROJECT_ROOT / src_file

    max_passes = 5
    for pass_num in range(1, max_passes + 1):
        print(f"  Pass {pass_num}...")
        changed = fix_file(src_file)
        if not changed:
            success, output = compile_file(src_file)
            if success:
                print(f"  OK: {src_file.name} compiles!")
            else:
                print(f"  Remaining errors in {src_file.name}")
            return
        print(f"  Applied call mismatch fixes")

    success, _ = compile_file(src_file)
    print(f"  {'OK' if success else 'FAIL'}: {src_file.name}")


if __name__ == "__main__":
    main()
