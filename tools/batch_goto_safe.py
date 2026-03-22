#!/usr/bin/env python3
"""
Batch goto elimination with compilation safety net.
Runs both goto_eliminator.py and goto_wholefile.py on each file,
then verifies compilation. Reverts if compilation breaks.
"""
import re
import os
import subprocess
import sys
import shutil
import traceback

# Files known to be broken - DO NOT TOUCH
BROKEN_FILES = {
    'src/game/battle/battle_logic.c',
    'src/game/gba/gba_conv.c',
    'src/game/gs_field_world.c',
    'src/game/gs_thread.c',
    'src/game/menu/menu_common_ext.c',
    'src/game/menu/menu_tool.c',
    'src/game/ui/ui_core.c',
    'src/hsd/hsd_cobj.c',
    'src/hsd/hsd_lobj.c',
}

def get_compiler_map():
    with open('build.ninja', 'r') as f:
        content = f.read()
    rules = re.findall(r'build\s+(\S+\.o)\s*:\s*(cc_GC_\w+)\s+(\S+\.c)', content)
    cmap = {}
    for obj, rule, src in rules:
        if '1_2_5n' in rule:
            cmap[src] = '1.2.5n'
        elif '1_3' in rule:
            cmap[src] = '1.3'
    return cmap

def count_gotos(filepath):
    with open(filepath, 'r', errors='ignore') as f:
        txt = f.read()
    return len(re.findall(r'\bgoto\s+\w+\s*;', txt))

def test_compile(src, ver):
    os.makedirs('build/test', exist_ok=True)
    outf = os.path.join('build', 'test', os.path.basename(src).replace('.c', '.o'))
    exe = os.path.join('tools', 'mwcc_compiler', 'GC', ver, 'mwcceppc.exe')
    cmd = f'{exe} -c -O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off -i include -o {outf} {src}'
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=120, shell=True)
    return result.returncode == 0

def run_eliminator(src):
    """Run goto_eliminator.py on a file."""
    try:
        result = subprocess.run(
            [sys.executable, 'tools/goto_eliminator.py', src],
            capture_output=True, text=True, timeout=300
        )
        return result.returncode == 0
    except Exception:
        return False

def run_wholefile(src):
    """Run goto_wholefile.py on a file."""
    try:
        result = subprocess.run(
            [sys.executable, 'tools/goto_wholefile.py', src],
            capture_output=True, text=True, timeout=300
        )
        return result.returncode == 0
    except Exception:
        return False

def main():
    cmap = get_compiler_map()

    # Find all files with gotos that are not broken
    goto_files = []
    for root, dirs, filenames in os.walk('src'):
        for fn in filenames:
            if fn.endswith('.c'):
                fp = os.path.join(root, fn).replace(os.sep, '/')
                if fp in BROKEN_FILES:
                    continue
                gotos = count_gotos(fp)
                if gotos > 0 and fp in cmap:
                    goto_files.append((gotos, fp))

    goto_files.sort(reverse=True)

    total_before = sum(g for g, _ in goto_files)
    total_removed = 0
    files_processed = 0

    print(f"Found {len(goto_files)} files with {total_before} total gotos to process\n", flush=True)

    for orig_gotos, fp in goto_files:
        ver = cmap[fp]

        # Verify it compiles before we start
        if not test_compile(fp, ver):
            print(f"SKIP (already broken): {fp} ({orig_gotos} gotos)", flush=True)
            continue

        # Read original content for backup
        with open(fp, 'r') as f:
            original_content = f.read()

        before = count_gotos(fp)

        try:
            # Run function-level eliminator first
            run_eliminator(fp)

            # Run whole-file eliminator
            run_wholefile(fp)

            # Run eliminators again for more passes
            for _ in range(5):
                prev = count_gotos(fp)
                run_eliminator(fp)
                run_wholefile(fp)
                curr = count_gotos(fp)
                if curr >= prev:
                    break

            after = count_gotos(fp)

            if after < before:
                # Test compilation
                if test_compile(fp, ver):
                    removed = before - after
                    total_removed += removed
                    files_processed += 1
                    print(f"OK  {fp}: {before} -> {after} (-{removed})", flush=True)
                else:
                    # Revert
                    with open(fp, 'w') as f:
                        f.write(original_content)
                    print(f"REVERT (compile fail): {fp} ({before} gotos)", flush=True)
            else:
                # No change, restore original
                with open(fp, 'w') as f:
                    f.write(original_content)
                print(f"SKIP (no change): {fp} ({before} gotos)", flush=True)
        except Exception as e:
            # Revert on any error
            with open(fp, 'w') as f:
                f.write(original_content)
            print(f"ERROR ({e}): {fp} ({before} gotos)", flush=True)
            traceback.print_exc()

    print(f"\n{'='*60}", flush=True)
    print(f"TOTAL: {total_removed} gotos removed from {files_processed} files", flush=True)
    print(f"Before: {total_before}, After: {total_before - total_removed}", flush=True)

if __name__ == '__main__':
    main()
