#!/usr/bin/env python3
"""run.py - one-command driver for the string-based symbol-map pipeline.

Runs, in order:
  0. mine_xrefs       dtk asm -> strings.json + fn_strings.json
  1. convert_splits   config/GC6E01/splits.txt -> old KNOWN/GAP row format
  2. attribute_tus    __FILE__ xrefs -> tu_attribution.md / tu_evidence.json
  3. propose_names    log/assert strings -> name_proposals.*
  4. port_xd          live XD (GXXE01) validate/port - a no-op unless XD
                      data has been wired up (see README).
  5. salvage_xd_archive  revalidate the archived campaign's XD/structural
                      port evidence against CURRENT symbols.txt - fills in
                      for stage 4 when no live XD data is available.
  6. build_symbol_map merge -> proposed_symbols.txt / symbols.with_proposals.txt

Defaults assume invocation from the repo root. Build the Colosseum asm first
with `python configure.py && ninja` (or it already exists in build/GC6E01/asm).
To enable a live XD port, extract XD's main.dol and split it (see README),
then pass --xd-asm and --xd-symbols.

PORTING NOTE (current tree, 2026-07): this is a straight port of the
archived campaign's tools/symbolmap/run.py. Stages 0/2/3/6 are byte-for-byte
the same evidence-mining logic as the archive; only asm-tree-layout and
splits-format plumbing changed (see each script's own porting note). Stages
1 and 5 are new plumbing to bridge those format differences without XD data.
"""

import argparse
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent


def run(script: str, *a) -> None:
    cmd = [sys.executable, str(HERE / script), *map(str, a)]
    print(f"\n$ {' '.join(str(x) for x in cmd)}")
    subprocess.run(cmd, check=True)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--asm-dir", type=Path, default=REPO / "build/GC6E01/asm")
    ap.add_argument("--out-dir", type=Path,
                    default=REPO / "config/GC6E01/symbolmap")
    ap.add_argument("--symbols", type=Path,
                    default=REPO / "config/GC6E01/symbols.txt")
    ap.add_argument("--splits", type=Path,
                    default=REPO / "config/GC6E01/splits.txt",
                    help="dtk-template splits.txt (auto-converted for "
                         "attribute_tus.py)")
    ap.add_argument("--archive-dir", type=Path,
                    default=REPO / "archive/previous_campaign/config/GC6E01/symbolmap",
                    help="archived campaign's symbolmap outputs, salvaged "
                         "when no live XD data is available")
    ap.add_argument("--xd-symbols", type=Path, default=REPO /
                    "tools/symbolmap/xd_ref/GXXE01/symbols.txt",
                    help="XD's config/GXXE01/symbols.txt (enables live "
                         "VALIDATE); see README 'Enabling The XD Port'")
    ap.add_argument("--xd-asm", type=Path, default=None,
                    help="XD asm dir from `dtk split` (enables live PORT)")
    ap.add_argument("--skip-salvage", action="store_true",
                    help="don't merge archived XD/structural evidence")
    args = ap.parse_args()

    run("mine_xrefs.py", "--asm-dir", args.asm_dir, "--out-dir", args.out_dir)

    splits_compat = args.out_dir / "splits_compat.txt"
    run("convert_splits.py", "--splits", args.splits, "--out", splits_compat)
    run("attribute_tus.py", "--sm-dir", args.out_dir, "--splits", splits_compat)

    run("propose_names.py", "--sm-dir", args.out_dir, "--symbols", args.symbols)

    if args.xd_asm and Path(args.xd_asm).is_dir():
        run("port_xd.py", "--sm-dir", args.out_dir,
            "--xd-symbols", args.xd_symbols,
            "--col-asm", args.asm_dir, "--xd-asm", args.xd_asm)
    else:
        run("port_xd.py", "--sm-dir", args.out_dir,
            "--xd-symbols", args.xd_symbols)

    if not args.skip_salvage:
        run("salvage_xd_archive.py", "--archive-dir", args.archive_dir,
            "--sm-dir", args.out_dir, "--symbols", args.symbols)

    run("build_symbol_map.py", "--sm-dir", args.out_dir,
        "--symbols", args.symbols)
    print("\n[done] see", args.out_dir)


if __name__ == "__main__":
    main()
