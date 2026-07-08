#!/usr/bin/env bash
# supervisor_daemon.sh — persistent 5-min loop wrapper around supervisor.sh,
# used because the box's root fs is full and cron cannot be written. Launched
# detached (setsid/nohup) with output on /storage. Self-single-flights via the
# supervisor's own flock.
BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
FARM="$BASE/farm"
export FARM_BASE="$BASE" WORKERS="${WORKERS:-20}" BUDGET="${BUDGET:-10800}"
echo "[$(date -u +%FT%TZ)] supervisor_daemon start pid=$$" >> "$FARM/logs/supervisor_daemon.log"
while :; do
  bash "$FARM/supervisor.sh" >> "$FARM/logs/supervisor.cron.log" 2>&1
  sleep 300
done
