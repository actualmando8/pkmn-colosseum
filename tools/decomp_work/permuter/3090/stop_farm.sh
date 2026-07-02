#!/usr/bin/env bash
# stop_farm.sh — halt all farm workers and their permuter children.
BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
FARM="$BASE/farm"
pkill -f "$FARM/worker.sh" 2>/dev/null
pkill -f "$BASE/decomp-permuter/permuter.py" 2>/dev/null
sleep 1
pkill -9 -f "$FARM/worker.sh" 2>/dev/null
pkill -9 -f "$BASE/decomp-permuter/permuter.py" 2>/dev/null
echo "farm stopped"
