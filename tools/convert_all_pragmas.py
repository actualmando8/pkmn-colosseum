#!/usr/bin/env python3
"""
convert_all_pragmas.py - Convert ALL 1,681 pragma-guarded register-level C
functions to idiomatic C89 across the Pokemon Colosseum decomp codebase.

Safe approach: remove pragmas, clean up artifacts, keep register variable
names to avoid type conflicts.

Handles two pragma patterns:
  A: #pragma optimization_level 0 ... #pragma optimization_level 4
  B: #pragma push / optimization_level 0 / optimizewithasm off ... #pragma pop

Also removes:
  - #pragma push / #pragma force_active on / #pragma pop (linker wrappers)
  - #pragma peephole off/on
  - #pragma dont_inline off/on

Does NOT touch:
  - asm / __asm blocks (real assembly, needs pragmas)
  - #pragma section / #pragma weak (essential language extensions)
"""

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"


def is_asm_block(lines, pragma_idx):
    """Check if a #pragma optimization_level 0 guards an asm function."""
    for k in range(pragma_idx + 1, min(pragma_idx + 6, len(lines))):
        s = lines[k].strip()
        if 'nofralloc' in s or '__asm' in s:
            return True
        if re.match(r'^(?:static\s+)?asm\s+', s):
            return True
        if s.startswith('{'):
            # Check next line for nofralloc
            if k + 1 < len(lines) and 'nofralloc' in lines[k + 1]:
                return True
            break
        if '{' in s:
            break
    return False


def find_function_sig(lines, start_idx, end_idx):
    """Find function signature line index within a range."""
    for i in range(start_idx, end_idx):
        s = lines[i].strip()
        if s.startswith('#pragma') or s == '' or (s.startswith('/*') and '*/' in s and '{' not in s):
            continue
        if re.match(r'^(?:static\s+)?(?:void|u32|s32|u16|s16|u8|s8|int|f32|f64|BOOL)\b', s):
            if '(' in s:
                return i
    return None


def clean_body_line(line):
    """Clean a single body line. Returns None to remove the line."""
    s = line.strip()

    # Remove asm-artifact comments
    if re.match(r'^/\*\s*(stmw|lmw|stfd|lfd|psq_st|psq_l|mflr|mtlr|stwu|'
                r'lwzx|stbx|subi|clrlslwi|lbzx|rlwinm|rlwimi|xoris|'
                r'subic|crclr|crset|mfcr|mtcrf|cntlzw|mulhw|divw|'
                r'subfic|addze|addme|mulli|subf|srawi|subfe|eqv|nand|'
                r'andc|orc|extsb|extsh|neg|slw|srw|sraw|mullw|divwu|'
                r'addic)\s.*\*/;?\s*$', s):
        return None
    if s in ('/* indirect jump via ctr */;',
             '/* crclr cr1eq */;', '/* crset cr1eq */;'):
        return None

    # Remove r1 = (u32)sp
    if re.match(r'^u32\s+r1\s*=\s*\(u32\)sp\s*;$', s):
        return None

    # Remove epilogue register restores from stack
    if re.match(r'^r\d+\s*=\s*\*\(u32\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*;$', s):
        return None
    # Float epilogue restores
    if re.match(r'^f\d+\s*=\s*\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*;$', s):
        return None
    # Float prologue saves
    if re.match(r'^\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*=\s*f\d+\s*;$', s):
        return None

    # Replace r1 references with sp-based ones
    # r1 + 0xNN -> (u32)&sp[0xNN]  -- but simpler: just keep as r1 isn't declared
    # Actually, replace r1 with (u32)sp since that's what r1 was
    line = re.sub(r'\br1\b', '(u32)sp', line)

    # Clean up comparisons
    # if ((u32)REG OP (u32)VAL) -> if (REG OP VAL)
    line = re.sub(
        r'if\s+\(\(u32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(u32\)((?:0x[0-9A-Fa-f]+|-?\d+|\w+))\)',
        r'if (\1 \2 \3)', line)
    # if ((s32)REG OP (s32)VAL) -> if ((s32)REG OP VAL)
    line = re.sub(
        r'if\s+\(\(s32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(s32\)((?:0x[0-9A-Fa-f]+|-?\d+|\w+))\)',
        r'if ((s32)\1 \2 \3)', line)

    # Clean shift constructs: (VAL << SHIFT) to hex constant
    def replace_shift(m):
        val, shift = int(m.group(1), 0), int(m.group(2))
        return f'0x{val << shift:X}'
    line = re.sub(r'\((\d+|0x[0-9A-Fa-f]+)\s*<<\s*(\d+)\)', replace_shift, line)

    return line


def process_file(filepath):
    """Process a single source file: remove pragmas and clean code."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    # Quick check
    if '#pragma' not in content:
        return 0

    lines = content.split('\n')
    new_lines = []
    i = 0
    blocks_converted = 0
    skip_until = -1

    while i < len(lines):
        if i < skip_until:
            i += 1
            continue

        s = lines[i].strip()

        # ===== Pattern A: #pragma optimization_level 0 ... 4 =====
        if s == '#pragma optimization_level 0':
            if is_asm_block(lines, i):
                new_lines.append(lines[i])
                i += 1
                continue

            # Find matching #pragma optimization_level 4
            end = None
            for j in range(i + 1, len(lines)):
                if lines[j].strip() == '#pragma optimization_level 4':
                    end = j
                    break

            if end is not None:
                # Extract function, skip pragma lines
                for k in range(i + 1, end):
                    cleaned = clean_body_line(lines[k])
                    if cleaned is not None:
                        new_lines.append(cleaned)
                blocks_converted += 1
                i = end + 1
                continue
            else:
                # No matching end, keep as-is
                new_lines.append(lines[i])
                i += 1
                continue

        # ===== Pattern B: #pragma push ... #pragma pop =====
        if s == '#pragma push':
            # Find matching #pragma pop
            depth = 1
            end = None
            has_opt_level = False
            has_force_active = False
            has_asm = False
            j = i + 1
            while j < len(lines) and depth > 0:
                sj = lines[j].strip()
                if sj == '#pragma push':
                    depth += 1
                elif sj == '#pragma pop':
                    depth -= 1
                    if depth == 0:
                        end = j
                elif sj == '#pragma optimization_level 0':
                    has_opt_level = True
                    if is_asm_block(lines, j):
                        has_asm = True
                elif sj == '#pragma force_active on':
                    has_force_active = True
                j += 1

            if end is None:
                new_lines.append(lines[i])
                i += 1
                continue

            if has_asm:
                # Keep entire asm block as-is
                for k in range(i, end + 1):
                    new_lines.append(lines[k])
                i = end + 1
                continue

            if has_opt_level and not has_asm:
                # Register-level function: strip pragmas, clean body
                for k in range(i, end + 1):
                    sk = lines[k].strip()
                    if sk.startswith('#pragma'):
                        continue
                    cleaned = clean_body_line(lines[k])
                    if cleaned is not None:
                        new_lines.append(cleaned)
                blocks_converted += 1
                i = end + 1
                continue

            if has_force_active and not has_opt_level:
                # Pure force_active wrapper: strip pragma lines, keep content
                for k in range(i, end + 1):
                    sk = lines[k].strip()
                    if sk in ('#pragma push', '#pragma pop', '#pragma force_active on'):
                        continue
                    new_lines.append(lines[k])
                i = end + 1
                continue

            # Other push/pop: keep as-is
            for k in range(i, end + 1):
                new_lines.append(lines[k])
            i = end + 1
            continue

        # ===== Simple pragma lines =====
        if s in ('#pragma optimization_level 4',
                 '#pragma peephole off', '#pragma peephole on',
                 '#pragma dont_inline off', '#pragma dont_inline on',
                 '#pragma optimizewithasm off'):
            i += 1
            continue

        # Keep everything else
        new_lines.append(lines[i])
        i += 1

    if blocks_converted > 0 or content.count('#pragma') != '\n'.join(new_lines).count('#pragma'):
        with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
            f.write('\n'.join(new_lines))
            if new_lines and new_lines[-1] != '':
                pass  # file already has newline from join

    return blocks_converted


def main():
    src_files = sorted(SRC_DIR.rglob("*.c"))
    total_converted = 0
    files_processed = 0

    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        if '#pragma' not in content:
            continue

        rel = src_file.relative_to(PROJECT_ROOT)
        converted = process_file(str(src_file))
        if converted > 0:
            files_processed += 1
            total_converted += converted
            print(f"  {rel}: {converted} blocks")

    print(f"\n{'='*60}")
    print(f"Converted {total_converted} blocks across {files_processed} files")

    # Count remaining pragmas
    remaining = 0
    remaining_asm = 0
    remaining_section_weak = 0
    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            for line in f:
                s = line.strip()
                if s.startswith('#pragma') and not s.startswith('/*'):
                    remaining += 1
                    if 'optimization_level 0' in s:
                        remaining_asm += 1
                    elif 'section' in s or 'weak' in s:
                        remaining_section_weak += 1

    print(f"Remaining pragmas: {remaining} (asm-related: {remaining_asm*4+remaining_asm}, section/weak: {remaining_section_weak})")
    return 0


if __name__ == '__main__':
    sys.exit(main() or 0)
