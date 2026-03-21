#!/usr/bin/env python3
"""
convert_remaining_pragmas.py - Handle remaining pragma blocks that the main
converter missed. Specifically:

1. #pragma push/force_active blocks wrapping single functions with
   optimization_level 0 + optimizewithasm off
2. Simple TODO stub functions wrapped in pragma push/pop
3. Files where the pragma block structure is non-standard

For asm/inline-asm blocks: these are kept as-is (they NEED the pragmas).
"""

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"


def process_force_active_file(filepath):
    """
    Handle files with #pragma push / #pragma force_active on / ... / #pragma pop
    wrapping register-level functions.

    Structure:
      #pragma push
      #pragma force_active on
      [possibly more pragmas]
      #pragma optimization_level 0
      #pragma optimizewithasm off
      void fn_XXXXX(...) {
          ... register-level body ...
      }
      #pragma pop

    We transform to:
      void fn_XXXXX(...) {
          ... cleaned body ...
      }
    """
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()
    lines = [l.rstrip('\n').rstrip('\r') for l in lines]

    changed = False
    i = 0
    new_lines = []

    while i < len(lines):
        s = lines[i].strip()

        # Pattern 1: #pragma push wrapping force_active + opt_level 0
        if s == '#pragma push':
            # Look ahead to see the structure
            block_end = None
            has_opt_level = False
            has_force_active = False
            has_asm = False
            has_todo = False
            depth = 1
            j = i + 1
            while j < len(lines) and depth > 0:
                sj = lines[j].strip()
                if sj == '#pragma push':
                    depth += 1
                elif sj == '#pragma pop':
                    depth -= 1
                    if depth == 0:
                        block_end = j
                elif sj == '#pragma optimization_level 0':
                    has_opt_level = True
                elif sj == '#pragma force_active on':
                    has_force_active = True
                elif sj.startswith('asm ') or 'asm void' in sj or 'asm u32' in sj or 'asm BOOL' in sj or 'static asm' in sj:
                    has_asm = True
                elif '__asm' in sj:
                    has_asm = True
                elif '{ /* TODO */' in sj:
                    has_todo = True
                j += 1

            if block_end is None:
                new_lines.append(lines[i])
                i += 1
                continue

            if has_asm:
                # Keep asm blocks as-is
                for k in range(i, block_end + 1):
                    new_lines.append(lines[k])
                i = block_end + 1
                continue

            if has_opt_level and not has_asm:
                # This is a register-level function wrapped in pragmas
                # Extract just the function, removing all pragma lines
                func_lines = []
                for k in range(i, block_end + 1):
                    sk = lines[k].strip()
                    if sk.startswith('#pragma'):
                        continue
                    func_lines.append(lines[k])

                # Clean the function body
                cleaned = clean_function_lines(func_lines)
                new_lines.extend(cleaned)
                changed = True
                i = block_end + 1
                continue

        # Pattern 2: Standalone #pragma optimization_level 0
        if s == '#pragma optimization_level 0':
            # Check if next line is optimizewithasm off
            if i + 1 < len(lines) and lines[i + 1].strip() == '#pragma optimizewithasm off':
                # Check if this is asm
                is_asm = False
                for k in range(i + 2, min(i + 5, len(lines))):
                    sk = lines[k].strip()
                    if sk.startswith('asm ') or 'asm void' in sk or 'asm u32' in sk or 'static asm' in sk:
                        is_asm = True
                        break
                    if '__asm' in sk:
                        is_asm = True
                        break

                if is_asm:
                    # Keep asm pragmas
                    new_lines.append(lines[i])
                    i += 1
                    continue
                else:
                    # Skip this pragma line (and optimizewithasm)
                    i += 2  # Skip both pragma lines
                    changed = True
                    continue
            else:
                # Standalone optimization_level 0 without optimizewithasm
                # Skip it
                changed = True
                i += 1
                continue

        # Pattern 3: #pragma optimizewithasm off (standalone)
        if s == '#pragma optimizewithasm off':
            # Skip (already handled above, but catch strays)
            changed = True
            i += 1
            continue

        new_lines.append(lines[i])
        i += 1

    if changed:
        with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
            for line in new_lines:
                f.write(line + '\n')

    return changed


def clean_function_lines(func_lines):
    """Clean a function extracted from a pragma block."""
    cleaned = []
    for line in func_lines:
        s = line.strip()

        # Skip asm-artifact comments
        if re.match(r'^/\*\s*(stmw|lmw|stfd|lfd|psq_st|psq_l|mflr|mtlr|stwu|'
                    r'lwzx|stbx|subi|clrlslwi|lbzx|rlwinm|rlwimi|xoris|'
                    r'subic|crclr|crset|mfcr|mtcrf|cntlzw|mulhw|divw|'
                    r'subfic|addze|addme|mulli|subf|srawi|subfe|eqv|nand|'
                    r'andc|orc|extsb|extsh|neg|slw|srw|sraw|mullw|divwu)\s.*\*/;?\s*$', s):
            continue
        if s in ('/* indirect jump via ctr */;',
                 '/* crclr cr1eq */;',
                 '/* crset cr1eq */;'):
            continue

        # Skip epilogue register restores
        if re.match(r'^r\d+\s*=\s*\*\(u32\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*;$', s):
            continue
        if re.match(r'^f\d+\s*=\s*\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*;$', s):
            continue

        # Skip r1 = (u32)sp
        if re.match(r'^r1\s*=\s*\(u32\)sp\s*;$', s):
            continue

        # Skip prologue float saves
        if re.match(r'^\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*=\s*f\d+\s*;$', s):
            continue

        # Transform comparisons
        s_orig = s
        # if ((u32)REG OP (u32)VAL) goto LABEL;
        m = re.match(r'^if\s+\(\(u32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(u32\)(0x[0-9A-Fa-f]+|-?\d+)\)\s*goto\s+(\w+)\s*;$', s)
        if m:
            reg, op, val, label = m.group(1), m.group(2), m.group(3), m.group(4)
            s = f'if ({reg} {op} {val}) goto {label};'

        # if ((s32)REG OP (s32)VAL) goto LABEL;
        m = re.match(r'^if\s+\(\(s32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(s32\)(0x[0-9A-Fa-f]+|-?\d+)\)\s*goto\s+(\w+)\s*;$', s)
        if m:
            reg, op, val, label = m.group(1), m.group(2), m.group(3), m.group(4)
            s = f'if ((s32){reg} {op} {val}) goto {label};'

        # if ((u32)REG OP (u32)REG2) goto LABEL;
        m = re.match(r'^if\s+\(\(u32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(u32\)(\w+)\)\s*goto\s+(\w+)\s*;$', s)
        if m:
            r1, op, r2, label = m.group(1), m.group(2), m.group(3), m.group(4)
            s = f'if ({r1} {op} {r2}) goto {label};'

        # if ((s32)REG OP (s32)REG2) goto LABEL;
        m = re.match(r'^if\s+\(\(s32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(s32\)(\w+)\)\s*goto\s+(\w+)\s*;$', s)
        if m:
            r1, op, r2, label = m.group(1), m.group(2), m.group(3), m.group(4)
            s = f'if ((s32){r1} {op} (s32){r2}) goto {label};'

        # Replace the content
        if s != s_orig:
            # Preserve indentation
            indent = len(line) - len(line.lstrip())
            cleaned.append(' ' * indent + s)
        else:
            cleaned.append(line)

    return cleaned


def main():
    src_files = sorted(SRC_DIR.rglob("*.c"))
    converted = 0

    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        if '#pragma optimization_level 0' not in content:
            continue

        # Skip files that only have asm blocks
        if 'asm void' in content or 'asm u32' in content or 'asm BOOL' in content or 'static asm' in content:
            # But check if there are ALSO non-asm blocks
            has_non_asm = False
            lines = content.split('\n')
            for idx, line in enumerate(lines):
                if line.strip() == '#pragma optimization_level 0':
                    # Check next few lines for asm
                    is_asm = False
                    for k in range(idx+1, min(idx+5, len(lines))):
                        sk = lines[k].strip()
                        if sk.startswith('asm ') or 'asm void' in sk or 'asm u32' in sk or 'asm BOOL' in sk or 'static asm' in sk or '__asm' in sk:
                            is_asm = True
                            break
                    if not is_asm:
                        has_non_asm = True
                        break
            if not has_non_asm:
                continue

        rel = src_file.relative_to(PROJECT_ROOT)
        result = process_force_active_file(str(src_file))
        if result:
            converted += 1
            print(f"  {rel}: cleaned")

    print(f"\nCleaned {converted} files")

    # Count remaining
    remaining = 0
    remaining_asm = 0
    remaining_other = 0
    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        count = content.count('#pragma optimization_level 0')
        if count > 0:
            remaining += count
            # Check if these are all asm
            lines = content.split('\n')
            for idx, line in enumerate(lines):
                if line.strip() == '#pragma optimization_level 0':
                    is_asm = False
                    for k in range(idx+1, min(idx+5, len(lines))):
                        sk = lines[k].strip()
                        if 'asm ' in sk or '__asm' in sk:
                            is_asm = True
                            break
                    if is_asm:
                        remaining_asm += 1
                    else:
                        remaining_other += 1

    print(f"Remaining #pragma optimization_level 0: {remaining} (asm={remaining_asm}, other={remaining_other})")


if __name__ == '__main__':
    sys.exit(main() or 0)
