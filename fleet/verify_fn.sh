#!/usr/bin/env bash
# verify_fn.sh <fn> [min_pct] — THE valid per-function match check for harvesting.
#
# CRITICAL: DOL byte-exactness is a dtk INVARIANT (the build falls back to the
# original asm for any non-matching function), so `sha1sum main.dol == orig` is
# ALWAYS true and is NOT a per-function match signal. The ONLY valid check is the
# function's objdiff fuzzy_match_percent from build/GC6E01/report.json.
set -uo pipefail
FN="${1:?usage: verify_fn.sh <fn> [min_pct]}"; MIN="${2:-100}"
cd "$(dirname "$0")/.."
ninja >/dev/null 2>&1 || { echo "BUILD FAILED"; exit 1; }
PCT=$(python3 - "$FN" <<'PY'
import json,sys
fn=sys.argv[1]; r=json.load(open('build/GC6E01/report.json'))
for u in r.get('units',[]):
    fns=list(u.get('functions',[]))
    for s in u.get('sections',[]): fns+=s.get('functions',[])
    for f in fns:
        if f.get('name')==fn: print(f.get('fuzzy_match_percent','?')); sys.exit()
print('NOTFOUND')
PY
)
echo "$FN: $PCT% (min $MIN)"
awk -v p="$PCT" -v m="$MIN" 'BEGIN{exit !(p+0>=m+0)}' 2>/dev/null && { echo "OK — landed"; exit 0; } || { echo "FAIL — revert"; exit 1; }
