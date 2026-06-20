#!/bin/bash
# fleet_driver.sh — keep the named lanes busy and harvest wins. Each cycle:
#   1. regenerate the bucket queue (gen_bucket_queue.py)
#   2. refill idle lanes (auto_rebatch.sh, scoped to the live lane list)
#   3. every GATE_EVERY cycles, gate+commit accumulated band wins (auto_gate.sh)
# Emits one status line only on a meaningful event (rebatch or commit) so it is a
# clean Monitor stream. The lane list is read from build/fleet_lanes.txt EACH cycle,
# so lanes can be added/removed live (e.g. bring Codex up at reset) without a restart.
# Default lanes "OPUS SON"; GLM stays out (out of commission).
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.." || exit 1
export MSYS_NO_PATHCONV=1
# SINGLETON GUARD: TaskStop kills the Monitor's grep but NOT this bash loop, so naive
# restarts pile up zombie drivers that all dispatch at once. Refuse to start a 2nd
# instance. Stale pidfile (after SIGKILL) is harmless — kill -0 on a dead pid is false.
PIDF="build/.fleet_driver.pid"
if [ -f "$PIDF" ] && kill -0 "$(cat "$PIDF" 2>/dev/null)" 2>/dev/null; then
  echo "[fleet_driver] another instance ($(cat "$PIDF")) already running — exiting"; exit 0
fi
echo $$ > "$PIDF"; trap 'rm -f "$PIDF"' EXIT
INTERVAL="${INTERVAL:-30}"
GATE_EVERY="${GATE_EVERY:-5}"
LANEFILE="build/fleet_lanes.txt"
[ -f "$LANEFILE" ] || echo "OPUS SON" > "$LANEFILE"
i=0; session=0
echo "[fleet_driver] up — lanes from $LANEFILE, interval ${INTERVAL}s, gate every ${GATE_EVERY}"
while true; do
  i=$((i+1))
  lanes=$(tr -d '\r' < "$LANEFILE" | tr '\n' ' ' | sed 's/  */ /g')
  [ -n "$(echo "$lanes" | tr -d ' ')" ] || lanes="OPUS SON"
  python tools/decomp_work/gen_bucket_queue.py >/tmp/fleet_q.txt 2>&1
  bucket=$(grep -oE "ACTIVE-BUCKET=[A-Z]+ files=[0-9]+" /tmp/fleet_q.txt | head -1)
  rb=$(ASM_LANES="$lanes" bash tools/decomp_work/auto_rebatch.sh 2>/dev/null | grep -c "^REBATCH")
  gatemsg=""
  if [ $((i % GATE_EVERY)) -eq 0 ]; then
    # Detect commits by HEAD change — robust to auto_gate's output format. auto_gate
    # only gates files not currently band-locked, so a lane camping a file delays its
    # wins until the lock releases; gating every GATE_EVERY cycles catches them then.
    before=$(git rev-parse HEAD 2>/dev/null)
    g=$(bash tools/decomp_work/auto_gate.sh 2>&1)
    after=$(git rev-parse HEAD 2>/dev/null)
    nfraud=$(echo "$g" | grep -ciE "fraud|reject")
    if [ -n "$before" ] && [ "$before" != "$after" ]; then
      nc=$(git rev-list --count "$before..$after" 2>/dev/null)
      nbe=$(echo "$g" | grep -oE "\+[0-9]+ byte-exact" | grep -oE "[0-9]+" | awk '{s+=$1} END{if(s)print s}')
      session=$((session + ${nc:-1}))
      gatemsg=" | GATED ${nc} commit(s) ${nbe:+(+${nbe} byte-exact)}"
    fi
    [ "$nfraud" -gt 0 ] && gatemsg="$gatemsg | fraud $nfraud"
  fi
  ts=$(date +%H:%M)
  if [ "$rb" -gt 0 ] || [ -n "$gatemsg" ]; then
    echo "[$ts] ${bucket:-bucket=?} | lanes:[$lanes] rebatched $rb$gatemsg | session +$session"
  fi
  sleep "$INTERVAL"
done
