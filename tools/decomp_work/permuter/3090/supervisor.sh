#!/usr/bin/env bash
# supervisor.sh — keep the permuter farm at full worker count and never let it
# wind down to zero. Run every few minutes from cron.
#
# Fixes two observed gaps:
#  1. Respawn: worker.sh exits permanently when claim.py returns empty (queue
#     drained) or on a transient error. This tops workers back up to WORKERS.
#  2. Re-admit: the queue is one-pass (each fn gets a terminal state; NOWIN is
#     never retried). When NO claimable work remains, retire the NOWIN states
#     for another permuter pass — more RNG seeds = fresh crack chances AND more
#     surrogate (Phase-2) training data. WIN / WIN_UNCONFIRMED / FAIL_BUILD stay
#     terminal (solved, or won't build). colosseum_battle stays parked-last via
#     the queue ordering, so a hand-worked lane isn't duplicated first.
set -uo pipefail
BASE="${FARM_BASE:-/storage/finetune/pkmn-colosseum-2026}"
FARM="$BASE/farm"
WORKERS="${WORKERS:-20}"
export FARM_BASE="$BASE"
export BUDGET="${BUDGET:-10800}"
LOCK="$FARM/state/.supervisor.lock"

# single-flight: never run two supervisors at once
exec 9>"$LOCK" || exit 0
flock -n 9 || exit 0

mkdir -p "$FARM/logs" "$FARM/state"
log() { echo "[$(date -u +%FT%TZ)] supervisor $*" >> "$FARM/logs/supervisor.log"; }

alive=$(pgrep -fc "$FARM/worker.sh" || true)
alive=${alive:-0}

# If nothing is claimable AND no worker is mid-job, re-admit NOWIN for a new pass.
if [ "$alive" -lt "$WORKERS" ]; then
  probe=$(flock "$FARM/state/.lock" python3 "$FARM/claim.py" wSUPERVISORPROBE 2>/dev/null)
  if [ -n "$probe" ]; then
    # we accidentally claimed one while probing — release it so a real worker gets it
    pf="${probe%%$'\t'*}"
    rm -f "$FARM/state/${pf}.status" 2>/dev/null
  else
    n=$(grep -l '^NOWIN' "$FARM/state/"*.status 2>/dev/null | wc -l)
    if [ "$n" -gt 0 ]; then
      grep -l '^NOWIN' "$FARM/state/"*.status 2>/dev/null | xargs -r rm -f
      log "queue exhausted — re-admitted $n NOWIN entries for another pass"
    fi
  fi
fi

# Top workers back up to WORKERS.
if [ "$alive" -lt "$WORKERS" ]; then
  need=$((WORKERS - alive))
  # derive worker ids already running so we don't collide tags
  mapfile -t used < <(pgrep -af "$FARM/worker.sh" | grep -oE 'worker\.sh [0-9]+' | awk '{print $2}')
  declare -A busy=(); for u in "${used[@]:-}"; do busy[$u]=1; done
  started=0; i=1
  while [ "$started" -lt "$need" ] && [ "$i" -le "$((WORKERS*2))" ]; do
    if [ -z "${busy[$i]:-}" ]; then
      setsid nohup nice -n 12 ionice -c3 bash "$FARM/worker.sh" "$i" \
          </dev/null >> "$FARM/logs/worker_$i.log" 2>&1 &
      busy[$i]=1; started=$((started+1)); sleep 1
    fi
    i=$((i+1))
  done
  log "respawned $started worker(s) (was $alive, target $WORKERS)"
fi
