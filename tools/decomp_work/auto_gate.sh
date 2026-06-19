#!/bin/bash
# auto_gate.sh — overnight autonomous gating. For every pending band-win tag:
# dry-run through the hardened band_integrate plug; if anything HELD, apply,
# reject asm/#if-flip fraud, skip redundant no-ops, and commit the real wins
# scoped (NEVER the 3 pre-existing WIP files). Prints a per-tag verdict + a total.
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.." || exit 1
PY=python
BI=tools/decomp_work/band_integrate.py
# the 3 pre-existing WIP files that are NOT this session's work — never commit them
WIP_RE='src/game/fsys/fsys_file.c|src/game/gs_pokemon_summary.c|src/hsd/hsd_dobj.c'
total=0
MARK=build/.last_gate
# Only consider tags banked/updated since the last pass (first pass: last 3h),
# so we don't re-dry-run dozens of already-gated historical tags every cycle.
if [ -f "$MARK" ]; then FILES=$(find build/band_wins -name 'pl_*.json' -newer "$MARK" 2>/dev/null)
else FILES=$(find build/band_wins -name 'pl_*.json' -mmin -180 2>/dev/null); fi
touch "$MARK"
for f in $FILES; do
  [ -f "$f" ] || continue
  tag=$(basename "$f" .json)
  nwins=$($PY -c "import json;print(sum(1 for k in json.load(open('$f')) if not k.startswith('_')))" 2>/dev/null || echo 0)
  [ "${nwins:-0}" -ge 1 ] || continue
  out=$($PY "$BI" "$tag" 2>&1)
  echo "$out" | grep -q "HELD" || { echo "  $tag: no-hold (walls/abort)"; continue; }
  $PY "$BI" "$tag" --apply >/dev/null 2>&1
  # changed src files from the accumulated applies, minus the WIP trio
  changed=$(git diff --name-only -- src/ 2>/dev/null | grep -vE "$WIP_RE")
  [ -n "$changed" ] || { echo "  $tag: redundant no-op (already in canon)"; continue; }
  # fraud guard: reject if any added line is asm storage / inline asm / .inc include
  fraud=0
  for src in $changed; do
    if git diff -- "$src" | grep -qiE "^\+.*(asm void |asm[[:space:]]*\{|__asm|#include[[:space:]]+\"[^\"]*\.inc\")"; then
      echo "  $tag: FRAUD in $src -> reverting"; git checkout -- "$src"; fraud=1
    fi
  done
  [ "$fraud" = 1 ] && continue
  changed=$(git diff --name-only -- src/ 2>/dev/null | grep -vE "$WIP_RE")
  [ -n "$changed" ] || continue
  held=$(echo "$out" | grep -c "HELD")
  committed=0
  for attempt in 1 2 3 4 5 6 7 8; do
    git add $changed build/matched_fns.txt tools/decomp_work/equivalent.txt 2>/dev/null
    if git commit -q -m "decomp: gate $tag (+$held byte-exact) [auto-overnight]

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"; then
      echo "  $tag: COMMITTED +$held"; total=$((total+held)); committed=1
      # mark committed fns attempted so the stale report.json doesn't re-offer them
      for cfn in $($PY -c "import json;print(' '.join(k for k in json.load(open('$f')) if not k.startswith('_')))" 2>/dev/null); do
        $PY tools/decomp_work/wall_ledger.py mark "$cfn" committed >/dev/null 2>&1
      done
      break
    fi
    sleep 4   # transient .git/objects permission race (AV scan) — back off and retry
  done
  [ "$committed" = 0 ] && echo "  $tag: COMMIT-FAILED after retries (win applied to canon, staged, NOT committed — manual commit needed)"
done
echo "=== auto_gate: +$total byte-exact committed this pass ==="
