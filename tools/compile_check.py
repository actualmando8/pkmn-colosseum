#!/usr/bin/env python3
"""
compile_check.py - Compile a decompiled source file and compare against the original.

Compiles the given .c file with mwcceppc using the appropriate flags for
Pokemon Colosseum (GCN), then optionally runs objdiff-cli to compare the
result against the original object file.

Usage:
    python tools/compile_check.py src/game/main.c
    python tools/compile_check.py src/crt/__init_cpp_exceptions.c --symbol main
    python tools/compile_check.py src/game/main.c --compiler-version 1.2.5n
    python tools/compile_check.py src/game/main.c --diff
    python tools/compile_check.py --all

The output .o file is placed at build/GC6E01/base/<relative_path>.o
where <relative_path> mirrors the source file path under src/.
"""

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path

# ============================================================================
# Project layout
# ============================================================================

PROJECT_ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = PROJECT_ROOT / "build" / "GC6E01"
BASE_DIR = BUILD_DIR / "base"
OBJ_DIR = BUILD_DIR / "obj"
SRC_DIR = PROJECT_ROOT / "src"
INCLUDE_DIR = PROJECT_ROOT / "include"
TOOLS_DIR = PROJECT_ROOT / "tools"

# Toolchain
MWCC_BASE = TOOLS_DIR / "mwcc_compiler"
MWCC_DEFAULT = MWCC_BASE / "mwcceppc.exe"
MWCC_GC_DIR = MWCC_BASE / "GC"
OBJDIFF_CLI = TOOLS_DIR / "objdiff-cli.exe"
OBJDIFF_JSON = PROJECT_ROOT / "objdiff.json"

# ============================================================================
# Compiler flag presets
# ============================================================================

# Default flags that match the original Colosseum build.
# CW version 8 (GC compiler 1.0 - 1.2.5n range).
# These are the most common GCN decompilation flags.
DEFAULT_CFLAGS = [
    "-O4,p",            # Full optimization with peephole
    "-nodefaults",      # Don't pull in default libraries
    "-proc", "gekko",   # Target Gekko (GCN) processor
    "-fp", "hard",      # Hardware floating point
    "-Cpp_exceptions", "off",  # No C++ exceptions
    "-enum", "int",     # Enums are int-sized
    "-warn", "off",     # Suppress warnings
    "-i", str(INCLUDE_DIR),   # Include path
]

# Per-module flag overrides. Some TUs were compiled with different settings.
# Keys are relative source paths (e.g., "crt/__init_cpp_exceptions.c").
MODULE_FLAGS = {
    # CRT modules may use slightly different optimization
    # Add overrides here as you discover them during matching.
}

# Per-module compiler version overrides.
# The MetroWerks CRT library was compiled with a newer compiler (1.3)
# than the game code (1.2.5n). Keys are relative source paths.
MODULE_COMPILER_VERSION = {
    "crt/global_destructor_chain.c": "1.3",
    "crt/exit.c": "1.3",
    "crt/printf.c": "1.3",
    "crt/stdio.c": "1.3",
    "crt/mem.c": "1.3",
    "crt/string.c": "1.3",
    "crt/__va_arg.c": "1.3",
}

# Default compiler version (GC subdir name).
# CW comment version 8 -> one of: 1.0, 1.1, 1.1p1, 1.2.5, 1.2.5n
# 1.2.5n is the most common for early GCN titles.
DEFAULT_COMPILER_VERSION = "1.2.5n"

# ============================================================================
# Source -> object mapping
# ============================================================================

def source_to_base_obj(src_path: Path) -> Path:
    """Convert a source file path to the base (recompiled) .o path.

    src/game/main.c -> build/GC6E01/base/game/main.o
    """
    try:
        rel = src_path.relative_to(SRC_DIR)
    except ValueError:
        # If the path isn't under src/, use just the filename
        rel = Path(src_path.stem)
    return BASE_DIR / rel.with_suffix(".o")


def find_target_obj(src_path: Path) -> Path:
    """Find the original (target) .o file that should contain code from src_path.

    For __init_cpp_exceptions.c, the target is __init_cpp_exceptions.o.
    For most other files, the code is in the monolithic auto_01_800055E0_text.o
    since dtk hasn't split individual TUs yet.
    """
    stem = src_path.stem
    rel = None
    try:
        rel = src_path.relative_to(SRC_DIR)
    except ValueError:
        pass

    # Check if there's a dedicated object file matching this source
    for obj_file in OBJ_DIR.glob("*.o"):
        if stem in obj_file.stem and "auto_" not in obj_file.stem:
            return obj_file

    # Check objdiff.json for explicit mapping
    if OBJDIFF_JSON.exists():
        try:
            with open(OBJDIFF_JSON) as f:
                cfg = json.load(f)
            rel_str = str(rel.with_suffix("")) if rel else stem
            for unit in cfg.get("units", []):
                unit_name = unit.get("name", "")
                if unit_name == rel_str or unit_name.endswith("/" + stem):
                    target = unit.get("target_path")
                    if target:
                        return PROJECT_ROOT / target
        except (json.JSONDecodeError, KeyError):
            pass

    # Default: the monolithic text object
    return OBJ_DIR / "auto_01_800055E0_text.o"


# ============================================================================
# Compilation
# ============================================================================

def get_compiler(version: str = None) -> Path:
    """Get the path to mwcceppc.exe for the given compiler version."""
    if version is None:
        version = DEFAULT_COMPILER_VERSION

    versioned = MWCC_GC_DIR / version / "mwcceppc.exe"
    if versioned.exists():
        return versioned

    # Fall back to the top-level compiler
    if MWCC_DEFAULT.exists():
        return MWCC_DEFAULT

    print(f"ERROR: No mwcceppc.exe found for version {version}")
    print(f"  Checked: {versioned}")
    print(f"  Checked: {MWCC_DEFAULT}")
    sys.exit(1)


def get_cflags(src_path: Path) -> list:
    """Get compiler flags for the given source file."""
    try:
        rel = str(src_path.relative_to(SRC_DIR))
    except ValueError:
        rel = src_path.name

    # Normalize to forward slashes for lookup
    rel = rel.replace("\\", "/")

    if rel in MODULE_FLAGS:
        return MODULE_FLAGS[rel]
    return list(DEFAULT_CFLAGS)


def compile_source(src_path: Path, compiler_version: str = None,
                   extra_flags: list = None, verbose: bool = False) -> Path:
    """Compile a single source file and return the output .o path.

    Returns:
        Path to the compiled .o file.
    Raises:
        SystemExit on compilation failure.
    """
    src_path = Path(src_path).resolve()
    if not src_path.exists():
        print(f"ERROR: Source file not found: {src_path}")
        sys.exit(1)

    out_obj = source_to_base_obj(src_path)
    out_obj.parent.mkdir(parents=True, exist_ok=True)

    # Check per-module compiler version override if not explicitly set
    if compiler_version is None:
        try:
            rel = str(src_path.relative_to(SRC_DIR)).replace("\\", "/")
        except ValueError:
            rel = src_path.name
        if rel in MODULE_COMPILER_VERSION:
            compiler_version = MODULE_COMPILER_VERSION[rel]

    compiler = get_compiler(compiler_version)
    cflags = get_cflags(src_path)

    if extra_flags:
        cflags.extend(extra_flags)

    cmd = [str(compiler), "-c", "-o", str(out_obj)] + cflags + [str(src_path)]

    if verbose:
        print(f"  Compiler: {compiler}")
        print(f"  Command:  {' '.join(cmd)}")
        print(f"  Output:   {out_obj}")

    result = subprocess.run(cmd, capture_output=True, text=True, cwd=str(PROJECT_ROOT))

    if result.returncode != 0:
        print(f"COMPILE FAILED (exit code {result.returncode})")
        if result.stdout.strip():
            print("--- stdout ---")
            print(result.stdout)
        if result.stderr.strip():
            print("--- stderr ---")
            print(result.stderr)
        sys.exit(1)

    if result.stderr.strip() and verbose:
        print("--- compiler warnings ---")
        print(result.stderr)

    if not out_obj.exists():
        print(f"ERROR: Compilation succeeded but output not found at {out_obj}")
        sys.exit(1)

    print(f"OK: {out_obj.relative_to(PROJECT_ROOT)}")
    return out_obj


# ============================================================================
# Diffing with objdiff-cli
# ============================================================================

def run_diff(src_path: Path, symbol: str = None, compiler_version: str = None,
             verbose: bool = False):
    """Compile a source file and diff it against the target object."""
    base_obj = compile_source(src_path, compiler_version=compiler_version,
                              verbose=verbose)
    target_obj = find_target_obj(src_path)

    if not target_obj.exists():
        print(f"WARNING: Target object not found: {target_obj}")
        print("  (Compile succeeded but cannot diff without original .o)")
        return

    if not OBJDIFF_CLI.exists():
        print(f"WARNING: objdiff-cli not found at {OBJDIFF_CLI}")
        print("  (Compile succeeded but cannot diff without objdiff-cli)")
        return

    cmd = [
        str(OBJDIFF_CLI), "diff",
        "-1", str(target_obj),
        "-2", str(base_obj),
    ]
    if symbol:
        cmd.append(symbol)

    if verbose:
        print(f"\n  Diff command: {' '.join(cmd)}")

    print(f"\nDiffing against: {target_obj.relative_to(PROJECT_ROOT)}")

    # Run in interactive mode (no -o flag) so user gets the TUI
    result = subprocess.run(cmd, cwd=str(PROJECT_ROOT))
    return result.returncode


def run_diff_json(src_path: Path, symbol: str = None,
                  compiler_version: str = None) -> dict:
    """Compile and diff, returning JSON results for programmatic use."""
    base_obj = compile_source(src_path, compiler_version=compiler_version)
    target_obj = find_target_obj(src_path)

    if not target_obj.exists() or not OBJDIFF_CLI.exists():
        return None

    cmd = [
        str(OBJDIFF_CLI), "diff",
        "-1", str(target_obj),
        "-2", str(base_obj),
        "-o", "-",
        "--format", "json",
    ]
    if symbol:
        cmd.append(symbol)

    result = subprocess.run(cmd, capture_output=True, text=True,
                            cwd=str(PROJECT_ROOT))
    if result.returncode != 0:
        print(f"objdiff error: {result.stderr}")
        return None

    try:
        return json.loads(result.stdout)
    except json.JSONDecodeError:
        return None


# ============================================================================
# Compile all source files
# ============================================================================

def compile_all(compiler_version: str = None, verbose: bool = False):
    """Compile every .c/.cpp file under src/."""
    src_files = sorted(SRC_DIR.rglob("*.c")) + sorted(SRC_DIR.rglob("*.cpp"))

    if not src_files:
        print("No source files found under src/")
        return

    print(f"Compiling {len(src_files)} source files...")
    print(f"  Compiler version: {compiler_version or DEFAULT_COMPILER_VERSION}")
    print()

    ok = 0
    fail = 0

    for src in src_files:
        rel = src.relative_to(SRC_DIR)
        try:
            compile_source(src, compiler_version=compiler_version, verbose=verbose)
            ok += 1
        except SystemExit:
            print(f"FAIL: {rel}")
            fail += 1

    print(f"\n{'='*50}")
    print(f"Results: {ok} succeeded, {fail} failed out of {len(src_files)} total")


# ============================================================================
# Entry point
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description="Compile decompiled source files for Pokemon Colosseum "
                    "and compare against originals.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python tools/compile_check.py src/game/main.c
  python tools/compile_check.py src/game/main.c --diff
  python tools/compile_check.py src/game/main.c --diff --symbol main
  python tools/compile_check.py src/game/main.c --compiler-version 1.1
  python tools/compile_check.py --all
        """,
    )

    parser.add_argument(
        "source", nargs="?",
        help="Source file to compile (e.g., src/game/main.c)"
    )
    parser.add_argument(
        "--all", action="store_true",
        help="Compile all source files under src/"
    )
    parser.add_argument(
        "--diff", action="store_true",
        help="After compiling, run objdiff-cli interactively"
    )
    parser.add_argument(
        "--symbol", "-s",
        help="Specific function symbol to diff"
    )
    parser.add_argument(
        "--compiler-version", "-cv", default=None,
        help=f"MWCC GC compiler version (default: {DEFAULT_COMPILER_VERSION}). "
             f"Available: see tools/mwcc_compiler/GC/"
    )
    parser.add_argument(
        "--verbose", "-v", action="store_true",
        help="Show detailed compilation commands"
    )

    args = parser.parse_args()

    if args.all:
        compile_all(compiler_version=args.compiler_version, verbose=args.verbose)
        return 0

    if not args.source:
        parser.print_help()
        print("\nERROR: Provide a source file or use --all")
        return 1

    src_path = Path(args.source)
    if not src_path.is_absolute():
        src_path = PROJECT_ROOT / src_path

    if args.diff:
        return run_diff(src_path, symbol=args.symbol,
                        compiler_version=args.compiler_version,
                        verbose=args.verbose)
    else:
        compile_source(src_path, compiler_version=args.compiler_version,
                       verbose=args.verbose)
        return 0


if __name__ == "__main__":
    sys.exit(main() or 0)
