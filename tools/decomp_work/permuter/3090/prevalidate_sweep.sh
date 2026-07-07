#!/bin/bash
# prevalidate_sweep.sh — fidelity-gate dry run over queue.tsv entries that are
# not already terminal/claimed. Report-only; workers rebuild dirs on claim.
B=/storage/finetune/pkmn-colosseum-2026
F=$B/farm
export TMPDIR=$F/tmp
out=$F/logs/prevalidate_refresh.tsv
: > "$out"
grep -v "^#" "$F/queue.tsv" | while IFS=$'\t' read -r tier pct size fn addr unit; do
  [ -z "$fn" ] && continue
  if [ -f "$F/state/$fn.status" ]; then echo -e "$fn\t$unit\tSKIP_STATE" >> "$out"; continue; fi
  res=$(python3 "$F/build_dir.py" "$fn" "$unit" 2>&1 | tail -1)
  echo -e "$fn\t$unit\t$res" >> "$out"
done
echo DONE >> "$out"
