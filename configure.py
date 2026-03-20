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
MWCC_BASE = TOOLS_DIR / "mwcc_compiler"
COMPILE_CONFIG_PATH = CONFIG_DIR / "compile_config.json"


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


def load_compile_config() -> dict:
    """Load per-file compiler configuration from compile_config.json."""
    if COMPILE_CONFIG_PATH.exists():
        try:
            with open(COMPILE_CONFIG_PATH, "r") as f:
                return json.load(f)
        except (json.JSONDecodeError, OSError) as e:
            print(f"WARNING: Failed to load {COMPILE_CONFIG_PATH}: {e}")
    return {
        "default": {
            "compiler": "GC/1.2.5n",
            "flags": "-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off",
        },
        "overrides": {},
    }


def check_mwcc():
    """Verify MetroWerks CodeWarrior compiler is available.

    Checks both the legacy flat path and the versioned paths referenced
    in compile_config.json.
    """
    compile_config = load_compile_config()

    # Collect all compiler versions we need
    versions_needed = set()
    default_compiler = compile_config.get("default", {}).get("compiler", "GC/1.2.5n")
    versions_needed.add(default_compiler)
    for override in compile_config.get("overrides", {}).values():
        if "compiler" in override:
            versions_needed.add(override["compiler"])

    all_found = True
    for version in sorted(versions_needed):
        versioned_path = MWCC_BASE / version / "mwcceppc.exe"
        if versioned_path.exists():
            print(f"  MWCC {version}: {versioned_path}")
        else:
            print(f"  WARNING: mwcceppc not found at {versioned_path}")
            all_found = False

    if not all_found:
        print("The MetroWerks compiler is required for matching decompilation.")
        print("See docs/setup.md for instructions on obtaining it.")

    return all_found


def check_dol():
    """Verify the original DOL exists."""
    dol_path = ORIG_DIR / "start.dol"
    if not dol_path.exists():
        print(f"ERROR: Original DOL not found at {dol_path}")
        print("Extract start.dol from your Pokémon Colosseum disc image.")
        print("See docs/setup.md for extraction instructions.")
        return False
    return True


def get_compiler_for_source(src_rel: str, compile_config: dict) -> str:
    """Determine the compiler version string for a source file.

    Args:
        src_rel: Source path relative to project root (e.g., 'src/crt/mem.c')
        compile_config: The loaded compile_config.json dict

    Returns:
        Compiler path segment like 'GC/1.3' or 'GC/1.2.5n'.
    """
    overrides = compile_config.get("overrides", {})
    if src_rel in overrides and "compiler" in overrides[src_rel]:
        return overrides[src_rel]["compiler"]
    return compile_config.get("default", {}).get("compiler", "GC/1.2.5n")


def get_flags_for_source(src_rel: str, compile_config: dict) -> str:
    """Determine the compiler flags for a source file.

    Args:
        src_rel: Source path relative to project root (e.g., 'src/crt/mem.c')
        compile_config: The loaded compile_config.json dict

    Returns:
        Flags string.
    """
    overrides = compile_config.get("overrides", {})
    if src_rel in overrides and "flags" in overrides[src_rel]:
        return overrides[src_rel]["flags"]
    return compile_config.get("default", {}).get(
        "flags",
        "-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off",
    )


def generate_ninja(compile_config: dict) -> str:
    """Generate build.ninja content with per-TU compiler version support.

    Creates a separate ninja rule for each distinct compiler version, then
    assigns each source file a build edge using the correct rule.
    """
    lines = []
    lines.append("# Auto-generated by configure.py — do not edit manually.")
    lines.append(f"# Project: {PROJECT_NAME} ({GAME_ID})")
    lines.append("")

    # Collect all compiler versions in use
    versions_used = set()
    default_compiler = compile_config.get("default", {}).get("compiler", "GC/1.2.5n")
    versions_used.add(default_compiler)
    for override in compile_config.get("overrides", {}).values():
        if "compiler" in override:
            versions_used.add(override["compiler"])

    # Emit a rule for each compiler version
    for version in sorted(versions_used):
        mwcc_path = MWCC_BASE / version / "mwcceppc.exe"
        # Sanitize the version for use as a ninja rule name (replace . and / with _)
        rule_name = "cc_" + version.replace("/", "_").replace(".", "_")
        lines.append(f"# MetroWerks CodeWarrior {version}")
        lines.append(f"rule {rule_name}")
        lines.append(f"  command = {mwcc_path} -c $cflags -i {INCLUDE_DIR} -o $out $in")
        lines.append(f"  description = CC ({version}) $in")
        lines.append("")

    # Discover all source files
    src_files = sorted(SRC_DIR.rglob("*.c")) + sorted(SRC_DIR.rglob("*.cpp"))

    if src_files:
        lines.append("# Build edges — one per translation unit")
        lines.append("")

    for src_file in src_files:
        src_rel = str(src_file).replace("\\", "/")
        try:
            rel_from_src = src_file.relative_to(SRC_DIR)
        except ValueError:
            rel_from_src = Path(src_file.name)

        out_obj = str((BUILD_DIR / "base" / rel_from_src.with_suffix(".o"))).replace("\\", "/")

        compiler_version = get_compiler_for_source(src_rel, compile_config)
        flags = get_flags_for_source(src_rel, compile_config)
        rule_name = "cc_" + compiler_version.replace("/", "_").replace(".", "_")

        lines.append(f"build {out_obj}: {rule_name} {src_rel}")
        lines.append(f"  cflags = {flags}")
        lines.append("")

    return "\n".join(lines) + "\n"


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

    # Load per-file compiler configuration
    compile_config = load_compile_config()

    print(f"  Config:      {config_path}")
    print(f"  Compile cfg: {COMPILE_CONFIG_PATH}")
    print(f"  Build dir:   {BUILD_DIR}")
    print(f"  DTK:         {DTK}")

    # Generate build.ninja with per-TU compiler rules
    ninja_content = generate_ninja(compile_config)
    ninja_path = Path("build.ninja")
    with open(ninja_path, "w") as f:
        f.write(ninja_content)
    print(f"  Generated {ninja_path}")

    # Show per-TU compiler assignments
    overrides = compile_config.get("overrides", {})
    if overrides:
        print(f"\n  Per-file compiler overrides ({len(overrides)} files):")
        for src_rel, info in sorted(overrides.items()):
            compiler = info.get("compiler", "(default)")
            print(f"    {src_rel:<45} -> {compiler}")

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
                f"config/{GAME_ID}/compile_config.json",
            ],
            "units": [],
        }

        with open(objdiff_path, "w") as f:
            json.dump(objdiff_config, f, indent=2)
            f.write("\n")
        print(f"  Generated objdiff.json (add units with tools/compile_check.py)")
    else:
        print(f"  objdiff.json already exists (not overwriting)")

    print("\nConfiguration complete.")
    if not has_mwcc:
        print("NOTE: Some mwcceppc versions not found — you can still analyze with dtk but may not compile all TUs.")

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
