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
    result = subprocess.run(
        [sys.executable, 'tools/goto_eliminator.py', src],
        capture_output=True, text=True, timeout=300
    )
    return result.returncode == 0

def run_wholefile(src):
    """Run goto_wholefile.py on a file."""
    result = subprocess.run(
        [sys.executable, 'tools/goto_wholefile.py', src],
        capture_output=True, text=True, timeout=300
    )
    return result.returncode == 0

def main():
    cmap = get_compiler_map()

    # Find all files with gotos that compile
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

    print(f"Found {len(goto_files)} files with {total_before} total gotos to process\n")

    for orig_gotos, fp in goto_files:
        ver = cmap[fp]

        # Verify it compiles before we start
        if not test_compile(fp, ver):
            print(f"SKIP (already broken): {fp}")
            continue

        # Save backup
        backup = fp + '.bak'
        shutil.copy2(fp, backup)

        before = count_gotos(fp)

        # Run function-level eliminator first
        run_eliminator(fp)
        after_elim = count_gotos(fp)

        # Run whole-file eliminator
        run_wholefile(fp)
        after_whole = count_gotos(fp)

        # Run eliminators again for more passes
        if after_whole < before:
            for _ in range(3):
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
                print(f"OK  {fp}: {before} -> {after} (-{removed})")
                os.remove(backup)
            else:
                # Revert
                shutil.copy2(backup, fp)
                os.remove(backup)
                print(f"REVERT (compile fail): {fp} ({before} gotos)")
        else:
            # No change, restore original
            shutil.copy2(backup, fp)
            os.remove(backup)
            print(f"SKIP (no change): {fp} ({before} gotos)")

    print(f"\n{'='*60}")
    print(f"TOTAL: {total_removed} gotos removed from {files_processed} files")
    print(f"Before: {total_before}, After: {total_before - total_removed}")

if __name__ == '__main__':
    main()
