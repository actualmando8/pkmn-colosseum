#!/usr/bin/env bash
# status.sh — one-line-ish farm status (run on the 3090).
BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
FARM="$BASE/farm"
S="$FARM/state"
total=$(grep -cv '^#' "$FARM/queue.tsv" 2>/dev/null || echo 0)
claimed=$(grep -l '^CLAIMED' "$S"/*.status 2>/dev/null | wc -l)
win=$(grep -l '^WIN ' "$S"/*.status 2>/dev/null | wc -l)
winu=$(grep -l '^WIN_UNCONFIRMED' "$S"/*.status 2>/dev/null | wc -l)
nowin=$(grep -l '^NOWIN' "$S"/*.status 2>/dev/null | wc -l)
fail=$(grep -l '^FAIL' "$S"/*.status 2>/dev/null | wc -l)
workers=$(pgrep -fc "$FARM/worker.sh" 2>/dev/null || echo 0)
perms=$(pgrep -fc "$BASE/decomp-permuter/permuter.py" 2>/dev/null || echo 0)
echo "workers=$workers permuters=$perms | queue=$total active=$claimed WIN=$win WIN?=$winu NOWIN=$nowin FAIL=$fail"
echo "--- active ---"
grep -H '^CLAIMED' "$S"/*.status 2>/dev/null | sed "s#$S/##;s#\.status:# #" | head -25
echo "--- recent terminal ---"
ls -t "$S"/*.status 2>/dev/null | head -10 | while read -r f; do
  head -1 "$f" | sed "s#^#$(basename "$f" .status): #"
done
