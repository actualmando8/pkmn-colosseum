#!/usr/bin/env python3
"""Disable source-invalid decomp-permuter mutations in every farm unit."""

import os
import re
import sys
from pathlib import Path


if len(sys.argv) != 2:
    raise SystemExit("usage: harden_permuter_settings.py ROOT")

root = Path(sys.argv[1])
unsafe = {
    "perm_dummy_comma_expr",
    "perm_pad_var_decl",
    "perm_duplicate_assignment",
    "perm_add_self_assignment",
    "perm_refer_to_var",
}
changed = 0
seen = 0
for path in root.glob("*/settings.toml"):
    seen += 1
    original = path.read_text(encoding="utf-8")
    text = original
    for key in unsafe:
        text = re.sub(
            rf"(?m)^(\s*{re.escape(key)}\s*=\s*)\d+(\s*)$",
            rf"\g<1>0\2",
            text,
        )
    if text != original:
        tmp = path.with_suffix(".toml.tmp")
        tmp.write_text(text, encoding="utf-8")
        os.replace(tmp, path)
        changed += 1

print(f"root={root} seen={seen} changed={changed}")
