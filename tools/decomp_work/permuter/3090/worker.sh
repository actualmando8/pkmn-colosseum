#!/usr/bin/env bash
# worker.sh <id> — one permuter farm worker on the 3090.
# Claims queue entries (flock-serialized), builds the permuter dir, runs
# simonlindholm's decomp-permuter for a bounded budget, harvests results.
set -uo pipefail
BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
FARM="$BASE/farm"
PERM="$BASE/decomp-permuter"
WID="${1:-0}"
BUDGET="${BUDGET:-10800}"          # seconds of permutation per function
export TMPDIR="$FARM/tmp"          # keep ALL temp I/O off the full root fs
export FARM_BASE="$BASE"
mkdir -p "$FARM/tmp" "$FARM/logs" "$FARM/state" "$FARM/results" "$FARM/dirs"

log() { echo "[$(date -u +%FT%TZ)] w$WID $*"; }

log "worker start budget=${BUDGET}s"
while :; do
  line=$(flock "$FARM/state/.lock" python3 "$FARM/claim.py" "w$WID") || break
  if [ -z "$line" ]; then
    log "queue drained — exiting"
    break
  fi
  IFS=$'\t' read -r fn unit <<< "$line"
  if [ -z "${fn:-}" ] || [ -z "${unit:-}" ]; then
    log "malformed claim line: $line"
    continue
  fi
  log "claim $fn ($unit)"
  if ! python3 "$FARM/build_dir.py" "$fn" "$unit" \
        > "$FARM/logs/build_${fn}.log" 2>&1; then
    rc=$?
    echo "FAIL_BUILD w$WID $(date +%s) rc=$rc" > "$FARM/state/${fn}.status"
    log "FAIL_BUILD $fn rc=$rc"
    continue
  fi
  DIR="$FARM/dirs/$fn"
  rm -rf "$DIR"/output-* 2>/dev/null || true
  log "permuting $fn (budget ${BUDGET}s)"
  timeout "$BUDGET" python3 "$PERM/permuter.py" "$DIR" -j 1 \
      --best-only --stop-on-zero \
      > "$FARM/logs/run_${fn}.log" 2>&1
  python3 "$FARM/harvest.py" "$fn" "$unit" "w$WID" >> "$FARM/logs/worker_${WID}.harvest.log" 2>&1
  log "done $fn -> $(head -1 "$FARM/state/${fn}.status" 2>/dev/null)"
done
log "worker exit"
