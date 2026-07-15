#!/usr/bin/env bash
# stop_farm.sh — halt the supervisor and every farm process group.
#
# Usage:
#   stop_farm.sh             stop without changing terminal queue state
#   stop_farm.sh --requeue   also release interrupted CLAIMED entries
#
# Workers and GNU timeout each create their own process groups. Killing only
# worker.sh can therefore orphan timeout/permuter/compiler children, and the
# supervisor daemon can immediately replace the worker. Stop all matching
# groups before optionally releasing claims.
set -uo pipefail

BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
FARM="$BASE/farm"
REQUEUE=0
case "${1:-}" in
  "") ;;
  --requeue) REQUEUE=1 ;;
  *) echo "usage: $0 [--requeue]" >&2; exit 2 ;;
esac

matching_pgids() {
  ps -eo pgid=,comm=,args= | awk \
    -v worker="$FARM/worker.sh" \
    -v supervisor="$FARM/supervisor_daemon.sh" \
    -v permuter="$BASE/decomp-permuter/permuter.py" '
      ($2 == "bash" && (index($0, worker) || index($0, supervisor))) ||
      ($2 == "timeout" && index($0, permuter)) ||
      ($2 ~ /^python/ && index($0, permuter)) { print $1 }
    ' | sort -nu
}

signal_groups() {
  local signal="$1" pgid
  while IFS= read -r pgid; do
    [ -n "$pgid" ] || continue
    kill "-$signal" -- "-$pgid" 2>/dev/null || true
  done < <(matching_pgids)
}

signal_groups TERM
sleep 1
signal_groups KILL
sleep 1

remaining=$(matching_pgids | wc -l | tr -d ' ')
released=0
if [ "$REQUEUE" -eq 1 ]; then
  for status in "$FARM"/state/*.status; do
    [ -f "$status" ] || continue
    if grep -q '^CLAIMED ' "$status"; then
      rm -f -- "$status"
      released=$((released + 1))
    fi
  done
fi

: > "$FARM/farm.pids"
echo "farm stopped: remaining=$remaining released_claims=$released"
[ "$remaining" -eq 0 ]
