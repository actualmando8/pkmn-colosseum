#!/bin/bash
# poll_win.sh — pull permuter wins from the Windows farm to the Mac.
#
# Copies outbox/<fn>/ (wins) and outbox/nearwins/ from the Windows box into
#   build/permuter_results/win/<fn>/           (modified C + summary.json)
#   build/permuter_results/win/nearwins/
# plus the farm heartbeat into build/permuter_results/win/status.json.
#
# NOTHING is committed — the wins wait for human validation.
#
# usage:  ./poll_win.sh            one pull
#         ./poll_win.sh --loop     pull every 5 min (Ctrl-C to stop)
#         ./poll_win.sh --status   just print farm status
set -uo pipefail

HOST=win
WIN_ROOT="C:/Users/douglaswhittingham/gamecube-decomp/pkmn-permuter"
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
DEST="$REPO/build/permuter_results/win"
mkdir -p "$DEST"

pull_once() {
  ts="$(date '+%Y-%m-%d %H:%M:%S')"

  # farm heartbeat (also proves the box is awake)
  if scp -q "$HOST:$WIN_ROOT/state/status.json" "$DEST/status.json" 2>/dev/null; then
    alive=$(python3 -c "import json;d=json.load(open('$DEST/status.json'));print(d.get('alive'),d.get('workers'),d.get('wins'),d.get('round'))" 2>/dev/null || echo "?")
    echo "[$ts] farm status: alive/workers/wins/round = $alive"
  else
    echo "[$ts] WARN: cannot reach farm status (box asleep or farm dir missing)"
    return 1
  fi

  # outbox → results (tar over ssh keeps it one round trip; bsdtar exists on
  # stock Windows).  The outbox only ever gains files, so a full copy is fine.
  if ssh "$HOST" "cd $WIN_ROOT && tar -cf - outbox" 2>/dev/null | tar -xf - -C "$DEST" 2>/dev/null; then
    # flatten outbox/ into DEST
    if [ -d "$DEST/outbox" ]; then
      rsync -a "$DEST/outbox/" "$DEST/" 2>/dev/null || cp -R "$DEST/outbox/". "$DEST/"
      rm -rf "$DEST/outbox"
    fi
    wins=$(find "$DEST" -mindepth 2 -maxdepth 2 -name summary.json -not -path "*/nearwins/*" | wc -l | tr -d ' ')
    near=$(find "$DEST/nearwins" -mindepth 2 -maxdepth 2 -name summary.json 2>/dev/null | wc -l | tr -d ' ')
    echo "[$ts] pulled: $wins wins, $near nearwins -> $DEST"
  else
    echo "[$ts] WARN: outbox pull failed"
  fi
}

case "${1:-}" in
  --status)
    if scp -q "$HOST:$WIN_ROOT/state/status.json" "$DEST/status.json" 2>/dev/null; then
      python3 -m json.tool "$DEST/status.json"
    else
      echo "farm unreachable"
    fi
    ;;
  --loop)
    while true; do
      pull_once || true
      sleep 300
    done
    ;;
  *)
    pull_once
    ;;
esac
