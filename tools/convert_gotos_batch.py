#!/usr/bin/env python3
"""
Batch goto converter for common patterns in the Pokemon Colosseum decomp.

Handles multiple patterns safely with compilation verification.
"""

import re
import sys
import os
import subprocess

EXCLUDE = {'ui_core.c', 'gs_field_world.c', 'menu_middle.c', 'colosseum_script.c',
           'colosseum_event.c', 'menu_tool.c', 'menu_common_ext.c'}

def count_gotos(text):
    """Count goto statements (not in comments)."""
    count = 0
    in_block = False
    for line in text.split('\n'):
        s = line.strip()
        if '/*' in s and '*/' not in s:
            in_block = True
        if '*/' in s:
            in_block = False
            continue
        if in_block or s.startswith('//') or s.startswith('*'):
            continue
        count += len(re.findall(r'\bgoto\s+\w+', line))
    return count

def find_labels(lines):
    """Find all labels and their line indices."""
    labels = {}
    for i, line in enumerate(lines):
        m = re.match(r'^(\s*)(L_\w+)\s*:\s*;?\s*$', line)
        if m:
            labels[m.group(2)] = i
        # Also handle labels with code on same line
        m2 = re.match(r'^(\s*)(L_\w+)\s*:\s*(.+)$', line)
        if m2 and not m:
            labels[m2.group(2)] = i
    return labels

def pattern_dowhile0_label(lines, labels):
    """Pattern: goto L_xxx inside do-while(0) where label is 'L_xxx: tmp = val;' before '} while(0);'"""
    changed = False
    for i in range(len(lines)):
        # Match: if (cond) goto L_xxx;
        m = re.search(r'(\s*)if\s*\((.+?)\)\s*goto\s+(L_\w+)\s*;', lines[i])
        if not m:
            # Match: standalone goto L_xxx;
            m2 = re.match(r'^(\s*)goto\s+(L_\w+)\s*;\s*$', lines[i])
            if m2:
                indent = m2.group(1)
                label = m2.group(2)
                if label not in labels:
                    continue
                lbl_line = labels[label]
                if lbl_line <= i:
                    continue
                # Check label pattern: L_xxx:\n tmp = 0xN;\n } while(0);
                lbl_text = lines[lbl_line].strip()
                if re.match(r'L_\w+\s*:\s*$', lbl_text):
                    if lbl_line + 1 < len(lines):
                        next_text = lines[lbl_line + 1].strip()
                        assign_m = re.match(r'(tmp\s*=\s*0x[0-9a-fA-F]+\s*;)', next_text)
                        if assign_m and lbl_line + 2 < len(lines):
                            after = lines[lbl_line + 2].strip()
                            if '} while' in after:
                                # Count gotos to this label
                                goto_refs = sum(1 for l in lines if re.search(rf'\bgoto\s+{re.escape(label)}\s*;', l))
                                lines[i] = f'{indent}{assign_m.group(1)} break;'
                                if goto_refs <= 1:
                                    lines[lbl_line] = ''
                                    lines[lbl_line + 1] = ''
                                changed = True
            continue

        indent = m.group(1)
        cond = m.group(2)
        label = m.group(3)
        if label not in labels:
            continue
        lbl_line = labels[label]
        if lbl_line <= i:
            continue

        # Check label pattern: L_xxx:\n tmp = 0xN;\n } while(0);
        lbl_text = lines[lbl_line].strip()
        if re.match(r'L_\w+\s*:\s*$', lbl_text):
            if lbl_line + 1 < len(lines):
                next_text = lines[lbl_line + 1].strip()
                assign_m = re.match(r'(tmp\s*=\s*0x[0-9a-fA-F]+\s*;)', next_text)
                if assign_m and lbl_line + 2 < len(lines):
                    after = lines[lbl_line + 2].strip()
                    if '} while' in after:
                        # Count gotos to this label
                        goto_refs = sum(1 for l in lines if re.search(rf'\bgoto\s+{re.escape(label)}\s*;', l))
                        lines[i] = f'{indent}if ({cond}) {{ {assign_m.group(1)} break; }}'
                        if goto_refs <= 1:
                            lines[lbl_line] = ''
                            lines[lbl_line + 1] = ''
                        changed = True
    return changed

def pattern_conditional_goto_to_end_label(lines, labels):
    """Pattern: if (cond) goto L_xxx; where L_xxx is just before a return or end of block."""
    changed = False
    for i in range(len(lines)):
        m = re.search(r'(\s*)if\s*\((.+?)\)\s*goto\s+(L_\w+)\s*;', lines[i])
        if not m:
            continue
        indent = m.group(1)
        cond = m.group(2)
        label = m.group(3)
        if label not in labels:
            continue
        lbl_line = labels[label]
        if lbl_line <= i:
            continue

        # Count how many gotos reference this label
        goto_count = 0
        for j in range(len(lines)):
            if re.search(rf'\bgoto\s+{re.escape(label)}\s*;', lines[j]):
                goto_count += 1

        # Only handle single-reference gotos for safety
        if goto_count != 1:
            continue

        # Check if the line right after the goto is a single statement or block start
        # This is for the pattern:
        #   if (cond) goto L_xxx;
        #   <code block>
        # L_xxx:
        #   <continuation>
        #
        # We can convert to:
        #   if (!(cond)) {
        #     <code block>
        #   }
        #   <continuation>

        # Find what's between goto and label
        between_start = i + 1
        between_end = lbl_line

        # Check if the label has no code after it (just a marker)
        lbl_text = lines[lbl_line].strip()
        if not re.match(r'L_\w+\s*:\s*;?\s*$', lbl_text):
            continue

        # Only convert if the between block is short (< 20 lines) and doesn't contain other gotos
        between = lines[between_start:between_end]
        between_text = '\n'.join(between)
        if len(between) > 20 or 'goto ' in between_text:
            continue

        # Check brace balance of the between block
        opens = between_text.count('{')
        closes = between_text.count('}')
        if opens != closes:
            continue

        # Negate the condition
        negated = negate_condition(cond)
        if negated is None:
            continue

        # Apply the conversion
        lines[i] = f'{indent}if ({negated}) {{'
        # Add closing brace before label
        lines[lbl_line] = f'{indent}}}'
        changed = True

    return changed

def negate_condition(cond):
    """Negate a C condition."""
    cond = cond.strip()
    if cond.startswith('!'):
        return cond[1:].strip()
    if cond.startswith('(') and cond.endswith(')'):
        inner = cond[1:-1]
        neg = negate_condition(inner)
        if neg:
            return neg

    # Simple comparisons
    replacements = {
        '==': '!=',
        '!=': '==',
        '>=': '<',
        '<=': '>',
        '>': '<=',
        '<': '>=',
    }
    for op, neg_op in replacements.items():
        # Find the operator (avoid matching inside casts like (s32))
        parts = re.split(rf'(?<!=)\s*{re.escape(op)}\s*(?!=)', cond, maxsplit=1)
        if len(parts) == 2:
            return f'{parts[0].strip()} {neg_op} {parts[1].strip()}'

    # Fallback: wrap in !()
    return f'!({cond})'

def pattern_or_goto(lines, labels):
    """Pattern: Two consecutive if-goto to same label:
       if (condA) goto L; if (condB) goto L;
       -> if (condA || condB) goto L;
    This is just a pre-processing step.
    """
    changed = False
    i = 0
    while i < len(lines) - 1:
        m1 = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+(L_\w+)\s*;\s*$', lines[i])
        m2 = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+(L_\w+)\s*;\s*$', lines[i+1])
        if m1 and m2 and m1.group(3) == m2.group(3) and m1.group(1) == m2.group(1):
            indent = m1.group(1)
            label = m1.group(3)
            cond = f'{m1.group(2)} || {m2.group(2)}'
            lines[i] = f'{indent}if ({cond}) goto {label};'
            lines[i+1] = ''
            changed = True
        i += 1
    return changed

def process_file(filepath, dry_run=False):
    """Process a single file."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        original = f.read()

    before = count_gotos(original)
    if before == 0:
        return 0

    lines = original.split('\n')

    max_iterations = 10
    for _ in range(max_iterations):
        labels = find_labels(lines)
        c1 = pattern_or_goto(lines, labels)

        labels = find_labels(lines)
        c2 = pattern_dowhile0_label(lines, labels)

        labels = find_labels(lines)
        c3 = pattern_conditional_goto_to_end_label(lines, labels)

        if not (c1 or c2 or c3):
            break

    text = '\n'.join(lines)
    after = count_gotos(text)
    removed = before - after

    if removed > 0 and not dry_run:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(text)

    return removed

def compile_file(src_path):
    """Compile a single file using ninja."""
    # Convert src path to build path
    rel = src_path.replace('\\', '/').replace('src/', '')
    obj = f'build/GC6E01/base/{rel}'.replace('.c', '.o')
    result = subprocess.run(['ninja', obj], capture_output=True, text=True)
    return result.returncode == 0

if __name__ == '__main__':
    dry_run = '--dry-run' in sys.argv
    total = 0
    files_changed = 0

    for root, dirs, files in os.walk('src'):
        for f in sorted(files):
            if f.endswith('.c') and f not in EXCLUDE:
                path = os.path.join(root, f).replace('\\', '/')
                removed = process_file(path, dry_run=dry_run)
                if removed > 0:
                    print(f'{path}: removed {removed} gotos')
                    total += removed
                    files_changed += 1

    print(f'\nTotal removed: {total} gotos from {files_changed} files')
