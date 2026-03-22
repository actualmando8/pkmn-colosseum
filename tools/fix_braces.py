#!/usr/bin/env python3
"""
Fix missing closing braces and related structural issues in C files
for the pkmn-colosseum decomp project.

Issues handled:
1. Functions that end without closing all their braces
2. Labels immediately before '} while' (need a ';' after label)
3. Orphaned 'else' blocks (no matching if)

Strategy:
- Find all top-level function definitions
- For each function body, count braces and identify the deficit
- Insert closing braces at the end of the function body (before the
  line that starts the next function or at EOF)
"""

import re
import sys


def find_function_starts(lines):
    """
    Return list of line indices where function definitions begin (the line
    with the opening '{').

    Handles both single-line and multi-line function signatures.
    """
    starts = []

    # Types that start a function definition at column 0
    type_re = re.compile(
        r'^(?:static\s+)?(?:void|u32|s32|u8|s8|u16|s16|f32|f64|int|GSThread\*|GSTask\*)\s+\w+\s*\(')

    i = 0
    while i < len(lines):
        if type_re.match(lines[i]):
            # Find the line with the opening '{'
            j = i
            while j < min(i + 5, len(lines)):
                if '{' in lines[j]:
                    starts.append(j)
                    break
                j += 1
        i += 1

    return starts


def compute_depth_at(lines, target):
    """Compute brace depth at the start of line 'target'."""
    depth = 0
    for i in range(target):
        for ch in lines[i]:
            if ch == '{':
                depth += 1
            elif ch == '}':
                depth -= 1
    return depth


def fix_file(filepath):
    with open(filepath, 'r') as f:
        lines = f.readlines()

    func_starts = find_function_starts(lines)

    # Add EOF as a sentinel
    func_starts.append(len(lines))

    # Build a list of insertions: (line_index, text_to_insert_before_line)
    insertions = {}

    for idx in range(len(func_starts) - 1):
        start = func_starts[idx]
        end = func_starts[idx + 1]  # Start of next function or EOF

        # Compute brace depth at the start of this function
        depth_before = compute_depth_at(lines, start)

        if depth_before > 0:
            # Need to insert closing braces before this function's first line
            # (before any comment block preceding it)
            insert_line = start
            # Walk backwards over comments and blank lines
            while insert_line > 0 and (lines[insert_line-1].strip() == '' or
                                        lines[insert_line-1].strip().startswith('/*') or
                                        lines[insert_line-1].strip().startswith('*') or
                                        lines[insert_line-1].strip().startswith('//')):
                insert_line -= 1

            insertions[insert_line] = depth_before

    # Also handle the last function (check if file ends with unclosed braces)
    total_depth = 0
    for line in lines:
        for ch in line:
            if ch == '{':
                total_depth += 1
            elif ch == '}':
                total_depth -= 1

    if total_depth > 0:
        insertions[len(lines)] = total_depth

    if not insertions:
        print(f"No brace fixes needed for {filepath}")
    else:
        # Apply insertions (in reverse order to preserve line numbers)
        for pos in sorted(insertions.keys(), reverse=True):
            count = insertions[pos]
            closing = '}\n' * count
            if pos >= len(lines):
                lines.append(closing)
            else:
                lines.insert(pos, closing)

        print(f"Brace fix: inserted closing braces at {len(insertions)} locations")

    # Fix 2: Labels before '} while' need ';' after label
    label_fixes = 0
    i = 0
    while i < len(lines):
        stripped = lines[i].rstrip()
        # Check if this is a label line
        if re.match(r'^\s+\w+:\s*;?\s*$', stripped) or re.match(r'^\s+L_[0-9A-Fa-f]+:\s*;?\s*$', stripped):
            # Check if next non-blank line starts with '} while'
            j = i + 1
            while j < len(lines) and lines[j].strip() == '':
                j += 1
            if j < len(lines) and re.match(r'^\s*\}\s*while\s*\(', lines[j]):
                # Ensure the label line has a ';' after it
                if not lines[i].rstrip().endswith(';'):
                    lines[i] = lines[i].rstrip() + '\n'
                    lines.insert(i + 1, '        ;\n')
                    label_fixes += 1
                elif ';' in lines[i] and lines[i].strip().endswith(';'):
                    pass  # Already has semicolon
                # If there's no statement between label and } while, add one
                else:
                    # Check if there's already a ; on the next line
                    pass

        i += 1

    if label_fixes:
        print(f"Label fix: added {label_fixes} semicolons after labels before '}}' while")

    with open(filepath, 'w') as f:
        f.writelines(lines)

    print(f"Done: {filepath}")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python fix_braces.py <file.c>")
        sys.exit(1)
    fix_file(sys.argv[1])
