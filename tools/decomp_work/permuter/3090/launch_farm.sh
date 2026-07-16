#!/usr/bin/env bash
# launch_farm.sh — start the permuter farm (idempotent; safe from cron @reboot).
set -uo pipefail
BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
FARM="$BASE/farm"
N="${WORKERS:-20}"
export BUDGET="${BUDGET:-10800}"

mkdir -p "$FARM/logs" "$FARM/state" "$FARM/results" "$FARM/dirs" "$FARM/tmp"

alive=$(pgrep -fc "$FARM/worker.sh" || true)
if [ "${alive:-0}" -gt 0 ]; then
  echo "farm already running ($alive workers) — not relaunching"
  exit 0
fi

: > "$FARM/farm.pids"
for i in $(seq 1 "$N"); do
  setsid nohup nice -n 12 ionice -c3 bash "$FARM/worker.sh" "$i" \
      </dev/null >> "$FARM/logs/worker_$i.log" 2>&1 &
  echo $! >> "$FARM/farm.pids"
  sleep 2   # stagger dir-build bursts
done
echo "launched $N workers (pids in $FARM/farm.pids), budget=${BUDGET}s/fn"

# Ensure the supervisor daemon is running (keeps workers topped up + re-admits
# improving NOWINs). Starting it here means the installed @reboot entry for
# launch_farm.sh also covers the supervisor across reboots. Idempotent.
if ! ps -eo cmd | grep -q "[s]upervisor_daemon.sh"; then
  setsid nohup nice -n 5 bash "$FARM/supervisor_daemon.sh" </dev/null >/dev/null 2>&1 &
  echo "started supervisor daemon"
fi
