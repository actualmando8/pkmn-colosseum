#!/usr/bin/env python3
"""Fix structural issues in decompiled C files.

Handles:
1. do {} without while -> add while (0);
2. Orphan } between functions -> remove
3. Functions with unbalanced braces -> add/remove closing braces
4. Orphaned 'break' outside loops -> wrap function body in do { } while(0)
"""

import re
import sys


def find_func_defs(lines):
    """Find function definition line indices."""
    func_defs = []
    for i, line in enumerate(lines):
        if re.match(r'^(void|u32|s32|u8|s8|u16|s16|f32|f64|void\*|f32\*)\s+fn_[0-9A-Fa-f]+\(', line):
            if '{' in line:
                func_defs.append(i)
    return func_defs


def remove_orphan_braces(lines):
    """Remove orphan } lines between functions."""
    func_defs = find_func_defs(lines)
    if not func_defs:
        return lines, 0

    to_remove = set()
    for idx, start in enumerate(func_defs):
        end = func_defs[idx + 1] if idx + 1 < len(func_defs) else len(lines)
        depth = 0
        closed_at = None
        for i in range(start, end):
            depth += lines[i].count('{') - lines[i].count('}')
            if depth == 0 and i > start and closed_at is None:
                closed_at = i

        if closed_at is not None:
            for i in range(closed_at + 1, end):
                stripped = lines[i].strip()
                if stripped == '}':
                    to_remove.add(i)

    if to_remove:
        lines = [l for i, l in enumerate(lines) if i not in to_remove]
    return lines, len(to_remove)


def fix_do_without_while(lines):
    """Fix do {} blocks that are missing their while clause."""
    changes = 0
    do_stack = []

    for i in range(len(lines)):
        stripped = lines[i].strip()

        if stripped.startswith('do {') and not stripped.startswith('//') and not stripped.startswith('/*'):
            do_stack.append(i)

        if '} while' in stripped and not stripped.startswith('//'):
            if do_stack:
                do_stack.pop()
        elif stripped == '}' and do_stack:
            do_indent = len(lines[do_stack[-1]]) - len(lines[do_stack[-1]].lstrip())
            cur_indent = len(lines[i]) - len(lines[i].lstrip())
            if cur_indent <= do_indent:
                indent = lines[i][:cur_indent]
                lines[i] = indent + '} while (0);\n'
                do_stack.pop()
                changes += 1

    return lines, changes


def fix_function_balance(lines):
    """Fix functions with unbalanced braces."""
    func_defs = find_func_defs(lines)
    changes = 0

    # First pass: fix functions with negative balance by removing the } that prematurely closes them
    for idx, start in enumerate(func_defs):
        end = func_defs[idx + 1] if idx + 1 < len(func_defs) else len(lines)
        depth = 0
        for i in range(start, end):
            depth += lines[i].count('{') - lines[i].count('}')

        if depth < 0:
            # Find where depth first reaches 0 (premature close)
            d = 0
            for i in range(start, end):
                d += lines[i].count('{') - lines[i].count('}')
                if d == 0 and i > start:
                    # Check if there's real code after this close point
                    has_code = False
                    for j in range(i + 1, end):
                        stripped = lines[j].strip()
                        if (stripped and
                            not stripped.startswith('/*') and
                            not stripped.startswith('*') and
                            not stripped.endswith('*/') and
                            stripped != '}' and
                            stripped != '} while (0);' and
                            not stripped.startswith('extern') and
                            j not in set(func_defs)):
                            has_code = True
                            break
                    if has_code and lines[i].strip() == '}':
                        lines[i] = '\n'
                        changes += 1
                    break

    # Re-find function defs after modifications
    func_defs = find_func_defs(lines)

    # Second pass: add missing closing braces for positive balance
    inserts = []
    for idx, start in enumerate(func_defs):
        end = func_defs[idx + 1] if idx + 1 < len(func_defs) else len(lines)
        depth = 0
        for i in range(start, end):
            depth += lines[i].count('{') - lines[i].count('}')
        if depth > 0:
            inserts.append((end, depth))

    for pos, count in sorted(inserts, reverse=True):
        for _ in range(count):
            lines.insert(pos, '}\n')
        changes += count

    return lines, changes


def has_orphaned_break(lines, start, end):
    """Check if a function has break statements outside any loop/switch."""
    loop_depth = 0
    # Simple tracking: count do/while/for/switch opens and closes
    for i in range(start + 1, end):  # skip function definition line
        stripped = lines[i].strip()

        # Track loop/switch entries
        if any(kw in stripped for kw in ['do {', 'while (', 'for (']):
            if '{' in stripped:
                loop_depth += 1
        if 'switch (' in stripped and '{' in stripped:
            loop_depth += 1

        # Track exits
        if '} while' in stripped:
            loop_depth = max(0, loop_depth - 1)
        elif stripped == '}' and loop_depth > 0:
            # This might close a loop body
            pass  # Imprecise but conservative

        # Check for orphaned break
        if stripped == 'break;' and loop_depth == 0:
            return True

    return False


def fix_orphaned_breaks(lines):
    """For functions with break outside loops, wrap body in do { } while (0)."""
    func_defs = find_func_defs(lines)
    changes = 0
    offsets = {}  # track line offsets from insertions

    for idx, start in enumerate(func_defs):
        end = func_defs[idx + 1] if idx + 1 < len(func_defs) else len(lines)

        # Check total balance first
        depth = 0
        for i in range(start, end):
            depth += lines[i].count('{') - lines[i].count('}')
        if depth != 0:
            continue  # Skip unbalanced functions

        # Find the function body range (after variable declarations, before return)
        # Find the first line of actual code (after declarations/externs)
        body_start = start + 1
        for i in range(start + 1, end):
            stripped = lines[i].strip()
            if (stripped.startswith('extern ') or
                stripped.startswith('u8 ') or stripped.startswith('u16 ') or
                stripped.startswith('u32 ') or stripped.startswith('s8 ') or
                stripped.startswith('s16 ') or stripped.startswith('s32 ') or
                stripped.startswith('f32 ') or stripped.startswith('f64 ') or
                stripped.startswith('void (*') or
                stripped == '' or stripped.startswith('/*') or stripped.startswith('*')):
                body_start = i + 1
            else:
                break

        # Check if there are orphaned breaks in this function
        # Use a simpler check: look for 'break;' at function body depth 1
        has_orphan = False
        d = 0
        for i in range(start, end):
            d += lines[i].count('{') - lines[i].count('}')
            stripped = lines[i].strip()
            if stripped == 'break;' and d == 1:
                # break at function body depth - orphaned
                has_orphan = True
                break

        if not has_orphan:
            continue

        # Find the closing } of the function
        d = 0
        func_close = end - 1
        for i in range(start, end):
            d += lines[i].count('{') - lines[i].count('}')
            if d == 0 and i > start:
                func_close = i
                break

        # Wrap body in do { } while (0);
        # Insert 'do {' after the declarations and '} while (0);' before the closing }
        # Find last return or last code line before closing }
        last_return = None
        for i in range(func_close - 1, body_start - 1, -1):
            stripped = lines[i].strip()
            if stripped == 'return;' or stripped.startswith('return '):
                last_return = i
                break
            elif stripped and not stripped.startswith('/*') and not stripped.startswith('*'):
                last_return = i
                break

        if last_return is None:
            continue

        # Insert do { after body_start and } while (0); before return
        indent = '    '
        lines.insert(last_return + 1, indent + '} while (0);\n')
        lines.insert(body_start, indent + 'do {\n')
        changes += 1

    return lines, changes


def fix_file(filepath):
    """Apply all fixes to a file."""
    with open(filepath, 'r') as f:
        lines = f.readlines()

    total_changes = 0

    # Fix do-without-while first
    lines, n = fix_do_without_while(lines)
    if n:
        print("  Fixed %d do-without-while" % n)
        total_changes += n

    # Remove orphan braces
    lines, n = remove_orphan_braces(lines)
    if n:
        print("  Removed %d orphan braces" % n)
        total_changes += n

    # Fix function balance (iterate)
    for _ in range(5):
        lines, n = fix_function_balance(lines)
        if n:
            print("  Fixed %d balance issues" % n)
            total_changes += n
        else:
            break

    # Fix orphaned breaks
    lines, n = fix_orphaned_breaks(lines)
    if n:
        print("  Wrapped %d functions with do-while for orphaned breaks" % n)
        total_changes += n

    with open(filepath, 'w') as f:
        f.writelines(lines)

    return total_changes


if __name__ == '__main__':
    files = sys.argv[1:]
    for f in files:
        print("Processing %s..." % f)
        changes = fix_file(f)
        print("  Total: %d changes\n" % changes)
