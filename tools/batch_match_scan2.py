#!/usr/bin/env python3
"""
batch_match_scan2.py - Optimized batch scan: one objdiff call per file.

For each .c file under src/game/:
  1. Compile it (fast - under 1 second).
  2. Run one whole-file objdiff diff producing JSON.
  3. Parse the right-side symbols and compute per-function match from instructions.
  4. Aggregate into per-file summary.

Outputs a sorted summary table.
"""

import json
import os
import subprocess
import sys
import time
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"
TOOLS_DIR = PROJECT_ROOT / "tools"
OBJDIFF_CLI = TOOLS_DIR / "objdiff-cli.exe"

sys.path.insert(0, str(TOOLS_DIR))
from compile_check import compile_source, find_target_obj
from headless_subprocess import run as run_tool


def scan_file(src_path: Path) -> dict:
    """Compile src, run one objdiff, parse all function matches."""
    rel = str(src_path.relative_to(PROJECT_ROOT)).replace("\\", "/")
    result = {
        "filename": rel,
        "functions_tested": 0,
        "functions_100pct": 0,
        "match_rate": 0.0,
        "error": None,
        "details": [],  # list of (name, pct, n_instructions)
    }

    # Compile
    try:
        base_obj = compile_source(src_path, verbose=False)
    except Exception as e:
        result["error"] = f"compile: {e}"
        return result

    target_obj = find_target_obj(src_path)
    if not target_obj.exists():
        result["error"] = "no_target_obj"
        return result

    # Run objdiff once for the whole file
    tmp_json = PROJECT_ROOT / "build" / "tmp_diff.json"
    cmd = [
        str(OBJDIFF_CLI), "diff",
        "-1", str(target_obj),
        "-2", str(base_obj),
        "-o", str(tmp_json),
        "--format", "json",
    ]

    try:
        res = run_tool(
            cmd, capture_output=True, text=True,
            cwd=str(PROJECT_ROOT), timeout=180,
        )
    except subprocess.TimeoutExpired:
        result["error"] = "objdiff_timeout"
        return result

    if res.returncode != 0:
        result["error"] = f"objdiff_rc{res.returncode}"
        return result

    if not tmp_json.exists():
        result["error"] = "no_json_output"
        return result

    # Parse JSON - can be large, stream-parse if needed
    try:
        with open(tmp_json, "r") as f:
            diff_data = json.load(f)
    except (json.JSONDecodeError, OSError) as e:
        result["error"] = f"json_parse: {e}"
        return result
    finally:
        try:
            tmp_json.unlink()
        except OSError:
            pass

    # Extract right-side (base / compiled source) symbols
    right_symbols = diff_data.get("right", {}).get("symbols", [])

    tested = 0
    matched_100 = 0
    match_sum = 0.0
    details = []

    for sym in right_symbols:
        name = sym.get("name", "")
        kind = sym.get("kind", "")

        # Skip section symbols like [.text]
        if name.startswith("[") or kind == "SYMBOL_SECTION":
            continue

        # Only consider function symbols
        if kind and kind != "SYMBOL_FUNCTION":
            continue

        instructions = sym.get("instructions", [])
        if not instructions:
            continue

        total_instr = len(instructions)
        matched_instr = 0
        for instr in instructions:
            dk = instr.get("diff_kind")
            if dk is None or dk == "DIFF_NONE":
                matched_instr += 1

        pct = 100.0 * matched_instr / total_instr if total_instr > 0 else 0.0

        tested += 1
        match_sum += pct
        if pct == 100.0:
            matched_100 += 1

        details.append((name, pct, total_instr))

    result["functions_tested"] = tested
    result["functions_100pct"] = matched_100
    result["match_rate"] = match_sum / tested if tested > 0 else 0.0
    result["details"] = details

    return result


def main():
    game_dir = SRC_DIR / "game"
    src_files = sorted(game_dir.rglob("*.c"))
    print(f"Found {len(src_files)} source files in src/game/", flush=True)
    print(f"{'=' * 85}", flush=True)

    results = []
    t0 = time.time()

    for i, src in enumerate(src_files, 1):
        rel = str(src.relative_to(PROJECT_ROOT)).replace("\\", "/")
        print(f"[{i:3d}/{len(src_files)}] {rel}...", end="", flush=True)
        t1 = time.time()

        r = scan_file(src)
        elapsed = time.time() - t1

        if r["error"]:
            print(f" ERR: {r['error']} ({elapsed:.1f}s)", flush=True)
        elif r["functions_tested"] == 0:
            print(f" 0 functions ({elapsed:.1f}s)", flush=True)
        else:
            print(f" {r['functions_tested']} funcs, "
                  f"{r['functions_100pct']} @100%, "
                  f"avg {r['match_rate']:.1f}% ({elapsed:.1f}s)", flush=True)

        results.append(r)

    total_time = time.time() - t0
    print(f"\n{'=' * 85}")
    print(f"All {len(src_files)} files scanned in {total_time:.0f}s\n")

    # ---- Summary table sorted by match_rate descending ----
    with_funcs = [r for r in results if r["functions_tested"] > 0]
    no_funcs = [r for r in results if r["functions_tested"] == 0]
    with_funcs.sort(key=lambda r: (-r["match_rate"], r["filename"]))

    print(f"{'FILENAME':<55} {'TESTED':>7} {'100%':>6} {'RATE':>8}")
    print(f"{'-'*55} {'-'*7} {'-'*6} {'-'*8}")

    grand_tested = 0
    grand_100 = 0
    grand_match_sum = 0.0

    for r in with_funcs:
        fn = r["filename"]
        if len(fn) > 54:
            fn = "..." + fn[-51:]
        print(f"{fn:<55} {r['functions_tested']:>7} "
              f"{r['functions_100pct']:>6} {r['match_rate']:>7.1f}%")
        grand_tested += r["functions_tested"]
        grand_100 += r["functions_100pct"]
        grand_match_sum += sum(pct for _, pct, _ in r["details"])

    if no_funcs:
        print(f"\n--- Files with 0 testable functions ---")
        for r in no_funcs:
            fn = r["filename"]
            err = r["error"] or "no functions found"
            print(f"{fn:<55} {err}")

    print(f"\n{'=' * 85}")
    overall = grand_match_sum / grand_tested if grand_tested else 0
    print(f"GRAND TOTAL: {grand_tested} functions tested, "
          f"{grand_100} at 100% "
          f"({100.0 * grand_100 / grand_tested:.1f}% perfect), "
          f"overall avg match {overall:.1f}%")
    print(f"Files with functions: {len(with_funcs)}/{len(results)}")

    return 0


if __name__ == "__main__":
    sys.exit(main() or 0)
