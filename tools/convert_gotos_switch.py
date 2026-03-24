#!/usr/bin/env python3
"""
Switch-case dispatch goto converter.

Handles decompiler-generated binary search dispatch patterns:
1. 'goto L' at end of scope where L is right after closing braces -> remove redundant goto
2. 'var = N; goto L_merge; ... L_case: var = M; goto L_merge; L_merge:' patterns
3. Conditional goto chains: 'if (c1) goto L; if (c2) goto L; ... goto L_default;'
"""
import re
import sys
import os
import subprocess
import shutil


def count_gotos(text):
    if isinstance(text, list):
        return sum(len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', l)) for l in text)
    return len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', text))


def compile_check(filepath):
    try:
        result = subprocess.run(
            [sys.executable, 'tools/compile_check.py', filepath],
            capture_output=True, text=True, timeout=120
        )
        return result.returncode == 0
    except Exception:
        return False


def build_label_info(lines):
    """Build label position and reference count maps."""
    label_pos = {}
    label_refs = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
        for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = gm.group(1)
            label_refs.setdefault(lbl, []).append(i)
    return label_pos, label_refs


def pass_goto_before_closing_to_label_after(lines):
    """Pattern: goto L; } ... } L: -> remove goto if control falls through braces to L.

    This handles: goto L_merge; } (closing brace) where L_merge is reachable
    by falling through all the closing braces.
    """
    changes = 0
    label_pos, label_refs = build_label_info(lines)

    for i in range(len(lines) - 1, -1, -1):
        m = re.match(r'^(\s*)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent_str = m.group(1)
        target = m.group(2)
        if target not in label_pos:
            continue
        target_pos = label_pos[target]
        if target_pos <= i:
            continue  # backward goto

        # Check if everything between goto and label is just closing braces,
        # empty lines, or semicolons (scope exits)
        all_structural = True
        for k in range(i + 1, target_pos):
            s = lines[k].strip()
            if not s or s == ';':
                continue
            if s == '}' or s == '} else {' or s.startswith('} while'):
                all_structural = False  # Has a while/else - can't fall through
                break
            if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', s):
                # Another label - can fall through if it's just a label
                continue
            all_structural = False
            break

        if not all_structural:
            # More strict check: are ALL lines between just '}'?
            only_braces = True
            for k in range(i + 1, target_pos):
                s = lines[k].strip()
                if not s or s == ';':
                    continue
                if s != '}':
                    only_braces = False
                    break
            if not only_braces:
                continue

        # Verify the brace count: from goto line to label, the braces should net close
        # but the label should be at the same or lower brace depth
        brace_depth = 0
        for k in range(i + 1, target_pos):
            brace_depth += lines[k].count('{') - lines[k].count('}')

        # If closing braces lead us to the label's scope, the goto is redundant
        if brace_depth <= 0:
            lines[i] = ''
            changes += 1
            # Check if label is now unreferenced
            refs = sum(1 for line in lines if f'goto {target}' in line)
            if refs == 0:
                lines[target_pos] = ''

    return changes


def pass_assign_goto_merge(lines):
    """Pattern: var = N; goto L_merge; ... L_case: var = M; goto L_merge; L_merge:

    When multiple paths set a variable then goto the same merge label,
    and the label itself has NO code after it (just the assignment pattern),
    these are switch-case results. We can't easily convert these without
    breaking the code structure, but we CAN remove redundant gotos when
    the merge label immediately follows.
    """
    changes = 0
    label_pos, label_refs = build_label_info(lines)

    for label, refs in label_refs.items():
        if label not in label_pos:
            continue
        pos = label_pos[label]

        # Check each ref: is it 'var = N; goto L;' where L is the next label?
        for ref_line in refs:
            s = lines[ref_line].strip()
            m = re.match(r'^goto ' + re.escape(label) + r';$', s)
            if not m:
                continue

            # Check if the label is right after this goto (just braces/empty between)
            only_braces = True
            for k in range(ref_line + 1, pos):
                sk = lines[k].strip()
                if not sk or sk == ';':
                    continue
                if sk == '}':
                    continue
                if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', sk):
                    continue
                only_braces = False
                break

            if only_braces and pos > ref_line:
                # Check brace depth
                brace_depth = 0
                for k in range(ref_line + 1, pos):
                    brace_depth += lines[k].count('{') - lines[k].count('}')
                if brace_depth <= 0:
                    lines[ref_line] = ''
                    changes += 1

    # Clean up unreferenced labels
    label_pos, label_refs = build_label_info(lines)
    for label, pos in list(label_pos.items()):
        if label not in label_refs or len(label_refs[label]) == 0:
            refs = sum(1 for line in lines if f'goto {label}' in line)
            if refs == 0:
                lines[pos] = ''
                changes += 1

    return changes


def pass_cond_goto_same_target_or(lines):
    """Combine: if (c1) goto L; if (c2) goto L; -> if (c1 || c2) goto L;

    Only when the two if-gotos are adjacent (no code between).
    """
    changes = 0
    i = 0
    while i < len(lines):
        m = re.match(r'^(\s+)if \((.+?)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            i += 1
            continue
        indent, cond, target = m.group(1), m.group(2), m.group(3)
        conds = [cond]
        j = i + 1
        while j < len(lines):
            s = lines[j].strip()
            if not s:
                j += 1
                continue
            mj = re.match(r'^' + re.escape(indent) + r'if \((.+?)\) goto ' + re.escape(target) + r';$', lines[j])
            if mj:
                conds.append(mj.group(1))
                j += 1
            else:
                break

        if len(conds) > 1:
            lines[i] = indent + 'if (' + ' || '.join(conds) + ') goto ' + target + ';'
            # Remove the subsequent lines
            for k in range(i + 1, j):
                if re.match(r'^\s+if \(.+?\) goto ' + re.escape(target) + r';$', lines[k]):
                    lines[k] = ''
                    changes += 1
            i = j
        else:
            i += 1
    return changes


def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    initial = count_gotos(content)
    if initial == 0:
        return 0

    lines = content.split('\n')

    for iteration in range(20):
        c = 0
        c += pass_cond_goto_same_target_or(lines)
        c += pass_goto_before_closing_to_label_after(lines)
        c += pass_assign_goto_merge(lines)
        if c == 0:
            break

    content = '\n'.join(lines)
    content = re.sub(r'\n{4,}', '\n\n\n', content)
    final = count_gotos(content)
    removed = initial - final

    if removed > 0:
        with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
            f.write(content)
        print(f'  {filepath}: {initial} -> {final} gotos ({removed} removed)')
    else:
        print(f'  {filepath}: no changes ({initial} gotos)')

    return removed


def main():
    if len(sys.argv) < 2:
        print("Usage: convert_gotos_switch.py <file.c> [...]")
        sys.exit(1)

    total = 0
    for filepath in sys.argv[1:]:
        if not os.path.exists(filepath):
            print(f'  File not found: {filepath}')
            continue

        backup = filepath + '.switch.bak'
        shutil.copy2(filepath, backup)

        removed = process_file(filepath)
        total += removed

        if removed > 0:
            if compile_check(filepath):
                print(f'  -> compile OK')
                os.remove(backup)
            else:
                print(f'  -> compile FAILED, reverting')
                shutil.copy2(backup, filepath)
                os.remove(backup)
                total -= removed
        else:
            os.remove(backup)

    print(f'\nTotal gotos removed: {total}')


if __name__ == '__main__':
    main()
