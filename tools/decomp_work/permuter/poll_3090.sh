#!/usr/bin/env bash
# poll_3090.sh — pull permuter-farm results from the 3090 box back to the Mac.
#
# Wins land in build/permuter_results/3090/<fn>/{source.c,diff.txt,summary.json}
# (partials under build/permuter_results/3090/_partials/). Read-only w.r.t. the
# remote; never touches src/ locally. The human validates & lands wins.
#
# usage: poll_3090.sh            one-shot pull + status print
#        poll_3090.sh --loop [N] poll every N seconds (default 300)
set -uo pipefail
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../../.." || exit 1

HOST="${PERMUTER_3090_HOST:-192.168.50.101}"
USER_HOST="${PERMUTER_3090_USER_HOST:-douglaswhittingham@$HOST}"
KEY="${PERMUTER_3090_KEY:-$HOME/.ssh/id_ed25519}"
RBASE="${PERMUTER_3090_BASE:-/storage/finetune/pkmn-colosseum-2026}"
DEST="build/permuter_results/3090"
mkdir -p "$DEST"

pull_once() {
  rsync -az -e "ssh -i $KEY -o ConnectTimeout=15" \
      "$USER_HOST:$RBASE/farm/results/" "$DEST/" 2>/dev/null
  ssh -i "$KEY" -o ConnectTimeout=15 "$USER_HOST" \
      "bash $RBASE/farm/status.sh" 2>/dev/null | tee "$DEST/.remote_status.txt"
  local wins
  wins=$(find "$DEST" -maxdepth 2 -name summary.json -not -path '*_partials*' | wc -l | tr -d ' ')
  echo "[poll_3090] $(date '+%H:%M:%S') results synced -> $DEST ($wins win dirs)"
}

if [ "${1:-}" = "--loop" ]; then
  INTERVAL="${2:-300}"
  echo "[poll_3090] polling $USER_HOST every ${INTERVAL}s (Ctrl-C to stop)"
  while :; do
    pull_once
    sleep "$INTERVAL"
  done
else
  pull_once
fi
