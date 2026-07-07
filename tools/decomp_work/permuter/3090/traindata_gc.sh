#!/usr/bin/env bash
# traindata_gc.sh — cap the permuter training-data dir at a byte budget.
# Deletes oldest rotated .gz first, then oldest inactive .jsonl, never the
# README. Run hourly from cron.
set -uo pipefail
BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
TD="$BASE/farm/traindata"
CAP_BYTES="${TRAINDATA_CAP_BYTES:-53687091200}"   # 50 GiB

[ -d "$TD" ] || exit 0
usage() { du -sb "$TD" | cut -f1; }

u=$(usage)
[ "$u" -le "$CAP_BYTES" ] && exit 0

# pass 1: oldest rotated gzips
find "$TD" -name "*.jsonl.*.gz" -printf "%T@ %p\n" 2>/dev/null | sort -n | cut -d" " -f2- | \
while read -r f; do
  rm -f "$f"
  [ "$(usage)" -le "$CAP_BYTES" ] && break
done

u=$(usage)
[ "$u" -le "$CAP_BYTES" ] && exit 0

# pass 2: oldest live jsonl not written in the last hour (inactive fns)
find "$TD" -name "*.jsonl" -mmin +60 -printf "%T@ %p\n" 2>/dev/null | sort -n | cut -d" " -f2- | \
while read -r f; do
  rm -f "$f"
  [ "$(usage)" -le "$CAP_BYTES" ] && break
done
