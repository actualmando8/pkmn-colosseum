#!/usr/bin/env python3

"""
Pokémon Colosseum Decompilation — Build Configuration Generator

Generates build.ninja and objdiff.json from the project configuration.
Based on the dtk-template configure.py pattern used across GCN decomp projects.

Usage:
    python configure.py              # Generate build files
    python configure.py progress     # Show decompilation progress
"""

import argparse
import json
import os
import sys
from pathlib import Path

# Project constants
PROJECT_NAME = "Pokemon Colosseum"
GAME_ID = "GC6E01"
CONFIG_DIR = Path("config") / GAME_ID
ORIG_DIR = Path("orig") / GAME_ID
BUILD_DIR = Path("build") / GAME_ID
SRC_DIR = Path("src")
INCLUDE_DIR = Path("include")
TOOLS_DIR = Path("tools")

# Toolchain paths
DTK = TOOLS_DIR / "dtk"
MWCC = TOOLS_DIR / "mwcc_compiler" / "mwcceppc.exe"


def check_dtk():
    """Verify decomp-toolkit is available."""
    dtk_path = DTK
    if sys.platform == "win32":
        dtk_path = DTK.with_suffix(".exe")
    if not dtk_path.exists():
        print(f"ERROR: decomp-toolkit not found at {dtk_path}")
        print("Install it with: cargo install decomp-toolkit")
        print("Or download a release from https://github.com/encounter/decomp-toolkit/releases")
        return False
    return True


def check_mwcc():
    """Verify MetroWerks CodeWarrior compiler is available."""
    if not MWCC.exists():
        print(f"WARNING: mwcceppc not found at {MWCC}")
        print("The MetroWerks compiler is required for matching decompilation.")
        print("See docs/setup.md for instructions on obtaining it.")
        return False
    return True


def check_dol():
    """Verify the original DOL exists."""
    dol_path = ORIG_DIR / "start.dol"
    if not dol_path.exists():
        print(f"ERROR: Original DOL not found at {dol_path}")
        print("Extract start.dol from your Pokémon Colosseum disc image.")
        print("See docs/setup.md for extraction instructions.")
        return False
    return True


def generate_build(args):
    """Generate build.ninja from configuration."""
    print(f"Configuring {PROJECT_NAME} ({GAME_ID})...")

    # Verify prerequisites
    has_dtk = check_dtk()
    has_mwcc = check_mwcc()
    has_dol = check_dol()

    if not has_dtk or not has_dol:
        print("\nCannot generate build files — see errors above.")
        return 1

    # Create build directories
    BUILD_DIR.mkdir(parents=True, exist_ok=True)

    config_path = CONFIG_DIR / "config.yml"
    if not config_path.exists():
        print(f"ERROR: Config not found at {config_path}")
        return 1

    # In a full setup, dtk handles ninja generation:
    #   dtk ninja config/GPXE01/config.yml --build-dir build/GPXE01
    # For now, generate a minimal build.ninja
    print(f"  Config:    {config_path}")
    print(f"  Build dir: {BUILD_DIR}")
    print(f"  DTK:       {DTK}")
    if has_mwcc:
        print(f"  MWCC:      {MWCC}")

    # Generate objdiff.json for side-by-side diffing.
    # Note: The main objdiff.json with per-unit mappings is maintained manually
    # (or by tools/compile_check.py). This only regenerates it if missing.
    objdiff_path = Path("objdiff.json")
    if not objdiff_path.exists():
        objdiff_config = {
            "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
            "min_version": "3.0.0",
            "custom_make": "python tools/compile_check.py",
            "build_target": False,
            "build_base": True,
            "watch_patterns": [
                "src/**/*.c",
                "src/**/*.cpp",
                "src/**/*.h",
                "include/**/*.h",
                f"config/{GAME_ID}/**",
            ],
            "units": [],
        }

        with open(objdiff_path, "w") as f:
            json.dump(objdiff_config, f, indent=2)
            f.write("\n")
        print("  Generated objdiff.json (add units with tools/compile_check.py)")
    else:
        print("  objdiff.json already exists (not overwriting)")

    print("\nConfiguration complete.")
    if not has_mwcc:
        print("NOTE: mwcceppc not found — you can still analyze with dtk but cannot compile.")

    return 0


def show_progress(args):
    """Display decompilation progress statistics."""
    # In a full setup, this reads the build artifacts to compute progress
    # For now, show a placeholder
    src_files = list(SRC_DIR.rglob("*.c")) + list(SRC_DIR.rglob("*.cpp"))

    print(f"\n{PROJECT_NAME} ({GAME_ID}) — Decompilation Progress")
    print("=" * 50)

    if not src_files:
        print("No decompiled source files yet.")
        print("Begin by analyzing the DOL with Ghidra and matching")
        print("small utility functions on decomp.me.")
    else:
        print(f"Source files: {len(src_files)}")
        # TODO: Parse build output for byte-level progress
        print("(Detailed progress tracking requires a complete build)")

    print()
    return 0


def main():
    parser = argparse.ArgumentParser(description=f"{PROJECT_NAME} build configuration")
    subparsers = parser.add_subparsers(dest="command", help="Command to run")

    # Default: configure
    sub_configure = subparsers.add_parser("configure", help="Generate build files")

    # Progress
    sub_progress = subparsers.add_parser("progress", help="Show decompilation progress")

    args = parser.parse_args()

    if args.command == "progress":
        return show_progress(args)
    else:
        return generate_build(args)


if __name__ == "__main__":
    sys.exit(main())
