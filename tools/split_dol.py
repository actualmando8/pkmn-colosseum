#!/usr/bin/env python3

###
# Run `dtk dol split` then apply the Pokémon Colosseum (GC6E01) byte-match
# ldscript fixup, as a SINGLE command.
#
# decomp-toolkit regenerates build/<version>/ldscript.lcf on every split with a
# hardcoded debug-stack gap of 0x2000. The retail DOL was linked with
# _db_stack_addr = _stack_addr + 0x8000 (verified byte-exact; see the legacy
# tools/decomp_work/build_dol.sh). dtk exposes no config knob for this, and ninja
# cannot portably chain `dtk ... && python ...` (cmd.exe passes `&&` straight to
# dtk), so the split + patch are wrapped here into one invocation.
#
# Invoked by tools/project.py's `split` rule:
#   $python tools/split_dol.py <dtk> <config.yml> <out_dir>
###

import subprocess
import sys
from pathlib import Path

DEBUG_STACK_FIX = ("_stack_addr + 0x2000", "_stack_addr + 0x8000")


def main() -> int:
    if len(sys.argv) < 4:
        print("usage: split_dol.py <dtk> <config.yml> <out_dir>", file=sys.stderr)
        return 2

    dtk, config_path, out_dir = sys.argv[1], sys.argv[2], sys.argv[3]

    # --no-update: a build must not rewrite the tracked symbols.txt/splits.txt
    # (dtk's in-place "update" churns them every run). Same as the legacy
    # build_dol.sh; the template's getting_started notes --no-update is "for
    # build systems". Detection still happens in-memory for the split.
    rc = subprocess.run(
        [dtk, "dol", "split", "--no-update", config_path, out_dir]
    ).returncode
    if rc != 0:
        return rc

    ldscript = Path(out_dir) / "ldscript.lcf"
    if ldscript.is_file():
        text = ldscript.read_text(encoding="utf-8")
        patched = text.replace(*DEBUG_STACK_FIX)
        if patched != text:
            ldscript.write_text(patched, encoding="utf-8")
            print(f"[split_dol] patched debug-stack gap 0x2000 -> 0x8000 in {ldscript}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
