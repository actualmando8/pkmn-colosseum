#!/bin/bash
# ship_win.sh — push generated work units to the Windows farm and (re)start it.
#
# Counterpart to poll_win.sh.  gen_workunits.py has already gated every unit
# locally (parse / compile / single-symbol / finite base score), so this only
# moves bytes and kicks the scheduled task.
#
# usage:  ./ship_win.sh <workunit-dir>          ship once
#         ./ship_win.sh <workunit-dir> --wait   retry every 5 min until the box
#                                               answers, then ship (for a farm
#                                               that is currently asleep)
set -uo pipefail

HOST=win
WIN_ROOT="C:/Users/douglaswhittingham/gamecube-decomp/pkmn-permuter"
SRC="${1:?usage: ship_win.sh <workunit-dir> [--wait]}"
WAIT="${2:-}"

[ -f "$SRC/manifest.json" ] || { echo "no manifest.json in $SRC"; exit 1; }
N=$(python3 -c "import json,sys;print(len(json.load(open('$SRC/manifest.json'))))")

ship_once() {
  ts="$(date '+%Y-%m-%d %H:%M:%S')"
  ssh -o ConnectTimeout=10 -o BatchMode=yes "$HOST" "echo ok" >/dev/null 2>&1 || {
    echo "[$ts] farm unreachable"
    return 1
  }
  echo "[$ts] shipping $N units from $SRC"
  # tar over ssh: one round trip, and bsdtar is stock on Windows.
  (cd "$SRC" && tar -cf - .) | ssh "$HOST" "cd $WIN_ROOT/units && tar -xf -" || return 1
  ssh "$HOST" "schtasks /end /tn PkmnPermuterFarm" >/dev/null 2>&1
  ssh "$HOST" "schtasks /run /tn PkmnPermuterFarm" || return 1
  echo "[$ts] shipped; farm restarted"
  return 0
}

if [ "$WAIT" = "--wait" ]; then
  while ! ship_once; do sleep 300; done
else
  ship_once
fi
