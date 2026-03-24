#!/usr/bin/env python3
"""
Specialized goto converter for do-while(0) switch dispatch patterns.

Pattern: Multiple 'goto L;' statements within a do{}while(0) block where L is
a label also inside the same block. These are switch-case merge points.

Strategy: For single-ref labels reachable only from within the same do-while(0),
convert to break + restructure. For multi-ref forward convergence labels where
all refs are forward unconditional gotos followed by different code paths,
wrap the preceding code in if-blocks.

Also handles: goto L where L is right after do-while(0) end -> break.
"""
import re
import sys
import os
import subprocess
import shutil


def count_gotos(text):
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


def find_dowhile0_blocks(lines):
    """Find do{}while(0) blocks and their line ranges."""
    blocks = []
    # Use a stack-based approach
    stack = []  # (line_index_of_do, brace_depth_at_do)
    global_depth = 0
    for i, line in enumerate(lines):
        s = line.strip()
        if s.startswith('do {') or s == 'do {':
            stack.append((i, global_depth))
        global_depth += line.count('{') - line.count('}')
        if ('} while (0);' in s or '} while(0);' in s) and stack:
            do_start, do_depth = stack.pop()
            blocks.append((do_start, i))
    return blocks


def pass_goto_to_break_after_dowhile(lines):
    """Convert 'goto L' where L is right after a do-while(0) end -> break."""
    changes = 0
    blocks = find_dowhile0_blocks(lines)

    label_pos = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i

    for do_start, do_end in blocks:
        # Find label right after do-while(0)
        target_label = None
        for k in range(do_end + 1, min(do_end + 4, len(lines))):
            s = lines[k].strip()
            if not s or s == ';':
                continue
            m = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
            if m:
                target_label = m.group(1)
            break

        if target_label is None:
            continue

        # Convert gotos to this label from within the do-while to break
        for k in range(do_start + 1, do_end):
            s = lines[k].strip()
            m_uncond = re.match(r'^(\s*)goto ' + re.escape(target_label) + r';$', lines[k])
            m_cond = re.match(r'^(\s*)if \((.+)\) goto ' + re.escape(target_label) + r';$', lines[k])
            if m_uncond:
                lines[k] = m_uncond.group(1) + 'break;'
                changes += 1
            elif m_cond:
                lines[k] = m_cond.group(1) + 'if (' + m_cond.group(2) + ') break;'
                changes += 1

    # Clean up labels that are now unreferenced
    for label, pos in list(label_pos.items()):
        refs = sum(1 for line in lines if 'goto ' + label in line)
        if refs == 0:
            m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[pos])
            if m:
                lines[pos] = ''
                changes += 1

    return changes


def pass_forward_uncond_goto_in_dowhile(lines):
    """Within do-while(0), convert forward unconditional goto where target label
    is also in the same do-while(0), and only referenced once."""
    changes = 0
    blocks = find_dowhile0_blocks(lines)

    for do_start, do_end in blocks:
        label_pos = {}
        label_refs = {}
        for k in range(do_start, do_end + 1):
            m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if m:
                label_pos[m.group(1)] = k
            for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k]):
                lbl = gm.group(1)
                label_refs.setdefault(lbl, []).append(k)

        # Process single-ref labels
        for label, refs in label_refs.items():
            if len(refs) != 1 or label not in label_pos:
                continue
            ref_line = refs[0]
            label_line = label_pos[label]
            if label_line <= ref_line:
                continue  # backward goto - skip

            # Check it's an unconditional goto
            m = re.match(r'^(\s*)goto ' + re.escape(label) + r';$', lines[ref_line])
            if not m:
                continue

            # Check nothing between goto and label has gotos to outside or labels from outside
            has_issue = False
            for k in range(ref_line + 1, label_line):
                s = lines[k].strip()
                if not s:
                    continue
                ml = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
                if ml:
                    inner_label = ml.group(1)
                    # Check if this inner label is referenced from outside the range
                    for j in range(len(lines)):
                        if (j < ref_line or j >= label_line) and f'goto {inner_label}' in lines[j]:
                            has_issue = True
                            break
                if has_issue:
                    break

            if has_issue:
                continue

            # The goto to label is a simple forward skip - just remove both
            # (the code between is dead code after the goto)
            all_dead = True
            for k in range(ref_line + 1, label_line):
                s = lines[k].strip()
                if s and not re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s):
                    all_dead = False
                    break

            if all_dead:
                lines[ref_line] = ''
                lines[label_line] = ''
                changes += 1

    return changes


def pass_redundant_goto_fallthrough(lines):
    """Remove gotos that jump to the very next non-empty line (fallthrough)."""
    changes = 0
    label_pos = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i

    for i in range(len(lines)):
        m = re.match(r'^(\s*)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        target = m.group(2)
        if target not in label_pos:
            continue
        target_pos = label_pos[target]
        # Check if everything between goto and label is empty
        all_empty = True
        for k in range(i + 1, target_pos):
            s = lines[k].strip()
            if s and s != ';':
                all_empty = False
                break
        if all_empty and target_pos > i:
            lines[i] = ''
            changes += 1
            # Remove label if no longer referenced
            refs = sum(1 for line in lines if 'goto ' + target in line)
            if refs == 0:
                lines[target_pos] = ''

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
        c += pass_redundant_goto_fallthrough(lines)
        c += pass_goto_to_break_after_dowhile(lines)
        c += pass_forward_uncond_goto_in_dowhile(lines)
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
        print("Usage: convert_gotos_dowhile.py <file.c> [...]")
        sys.exit(1)

    total = 0
    for filepath in sys.argv[1:]:
        if not os.path.exists(filepath):
            print(f'  File not found: {filepath}')
            continue

        backup = filepath + '.dowhile.bak'
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
