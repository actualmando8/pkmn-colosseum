#!/bin/bash
# ship_win.sh — push generated work units to the Windows farm.
#
# Counterpart to poll_win.sh.  gen_workunits.py has already gated every unit
# locally (parse / compile / single-symbol / finite base score), so this only
# moves bytes.
#
# Two things this is careful about, both learned the hard way:
#
#   * `units/manifest.json` is the farm's whole queue, and farm.py *prunes
#     persisted results for any unit missing from it*.  Overwriting it with a
#     one-campaign manifest silently throws away every other campaign's
#     accumulated best scores.  So the remote manifest is merged, not replaced.
#   * `status: skip` units failed their own fidelity gate.  farm.py ignores
#     them, but there is no reason to ship them.
#
# The farm reloads the manifest between worker assignments, so a running farm
# picks new units up on its own; the scheduled task is only started when the
# farm is not already alive (restarting would discard in-flight worker
# progress).
#
# usage:  ./ship_win.sh <workunit-dir>          ship once
#         ./ship_win.sh <workunit-dir> --wait   retry every 5 min until the box
#                                               answers (for a sleeping farm)
set -uo pipefail

HOST=win
WIN_ROOT="C:/Users/douglaswhittingham/gamecube-decomp/pkmn-permuter"
SRC="${1:?usage: ship_win.sh <workunit-dir> [--wait]}"
WAIT="${2:-}"

[ -f "$SRC/manifest.json" ] || { echo "no manifest.json in $SRC"; exit 1; }

ship_once() {
  local ts tmp
  ts="$(date '+%Y-%m-%d %H:%M:%S')"
  ssh -o ConnectTimeout=10 -o BatchMode=yes "$HOST" "echo ok" >/dev/null 2>&1 || {
    echo "[$ts] farm unreachable"
    return 1
  }

  tmp="$(mktemp -d)"
  trap 'rm -rf "$tmp"' RETURN

  # ok units only
  python3 - "$SRC/manifest.json" > "$tmp/ok.txt" <<'PY' || return 1
import json, sys
for e in json.load(open(sys.argv[1])):
    if e.get("status") == "ok":
        print(e["fn"])
PY
  local n; n=$(wc -l < "$tmp/ok.txt" | tr -d ' ')
  [ "$n" -gt 0 ] || { echo "[$ts] no ok units in $SRC"; return 1; }
  echo "[$ts] shipping $n units from $SRC"
  (cd "$SRC" && tar -cf - -T "$tmp/ok.txt") | ssh "$HOST" "cd $WIN_ROOT/units && tar -xf -" || return 1

  # merge manifests: remote entries win for units we are not re-shipping
  ssh "$HOST" "cmd /c type ${WIN_ROOT//\//\\}\\units\\manifest.json" 2>/dev/null > "$tmp/remote.json" || :
  python3 - "$SRC/manifest.json" "$tmp/remote.json" > "$tmp/merged.json" <<'PY' || return 1
import json, sys
new = [e for e in json.load(open(sys.argv[1])) if e.get("status") == "ok"]
try:
    old = json.load(open(sys.argv[2]))
except Exception:
    old = []
fresh = {e["fn"] for e in new}
merged = [e for e in old if e.get("status") == "ok" and e["fn"] not in fresh] + new
json.dump(merged, sys.stdout, indent=1)
print(f"merged manifest: {len(merged)} units "
      f"({len(merged) - len(new)} kept + {len(new)} shipped)", file=sys.stderr)
PY
  scp -q "$tmp/merged.json" "$HOST:$WIN_ROOT/units/manifest.json" || return 1

  if ssh "$HOST" "cmd /c type ${WIN_ROOT//\//\\}\\state\\status.json" 2>/dev/null \
       | grep -q '"alive": true'; then
    echo "[$ts] farm already running; it reloads the manifest between assignments"
  else
    ssh "$HOST" "schtasks /run /tn PkmnPermuterFarm" || return 1
    echo "[$ts] farm started"
  fi
  return 0
}

if [ "$WAIT" = "--wait" ]; then
  while ! ship_once; do sleep 300; done
else
  ship_once
fi
