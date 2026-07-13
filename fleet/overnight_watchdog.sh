#!/usr/bin/env bash
# Overnight autonomy watchdog (rebuilt 2026-07-10 after host crash).
# Every 8 min: reap hung kernel sessions, restart dead lanes/loops, recover
# phantom claims, prune stale worktrees, log a productivity trail.
HARNESS=/Users/douglaswhittingham/gamecube-decomp-harness
GAME=/Users/douglaswhittingham/pkmn-colosseum
FLEET=${FLEET_DIR:-$GAME/fleet}
RUNTIME_PATH=${FLEET_RUNTIME_PATH:-/Users/douglaswhittingham/.bun/bin:/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin}
export PATH="$RUNTIME_PATH"
DB="$HARNESS/projects/pkmn-colosseum/state/orchestrator.sqlite"
LOG=/tmp/grind/watchdog.log
PAUSE_FILE=/tmp/grind/harness-paused.txt
FS_PUSH_SCRIPT="$FLEET/fs_push.sh"
source "$FLEET/runtime.sh"
cd "$HARNESS"
mkdir -p /tmp/grind

fleet_is_paused() {
  [ -f "$PAUSE_FILE" ]
}

while true; do
  ts=$(date '+%m-%d %H:%M')
  paused=0
  restarted_lanes=
  fleet_is_paused && paused=1
  small_run=$(fleet_ensure_run_id small 2>/dev/null || true)
  medium_run=$(fleet_ensure_run_id medium 2>/dev/null || true)
  large_run=$(fleet_ensure_run_id large 2>/dev/null || true)
  # 1. reap hung kernel sessions (>45 min = past any rung budget) -> frees worker slots
  if ! fleet_is_paused; then
    reaped=$(docker exec agent-kernel-db psql -U agent_kernel -d agent_kernel -tAc \
      "WITH x AS (UPDATE pi_agent_sessions SET status='error' WHERE status='running' AND created_at < now() - interval '45 minutes' RETURNING 1) SELECT count(*) FROM x;" 2>/dev/null | tr -d ' ')
    # 1c. codex-spark specific: reap sessions >20min (its budget) so it never wedges a slot.
    if ! fleet_is_paused; then
      docker exec agent-kernel-db psql -U agent_kernel -d agent_kernel -tAc "UPDATE pi_agent_sessions SET status='error' WHERE status='running' AND model='gpt-5.3-codex-spark' AND created_at < now() - interval '20 minutes';" >/dev/null 2>&1
    else
      paused=1
    fi
  else
    paused=1
    reaped=paused
  fi
  # productivity trail: finished/size per lane
  prod=""
  for pr in "sm:$small_run" "md:$medium_run" "lg:$large_run"; do
    nm=${pr%%:*}; rr=${pr#*:}; [ -z "$rr" ] && continue
    if [[ "$rr" == PAUSED-* ]]; then
      prod="$prod $nm:paused"
      continue
    fi
    e=$(sqlite3 "$DB" "SELECT finished_count||'/'||size_value FROM epochs WHERE session_id='$rr' ORDER BY ordinal DESC LIMIT 1;" 2>/dev/null)
    w=$(sqlite3 "$DB" "SELECT count(*) FROM worker_state ws JOIN epoch_targets et ON et.id=ws.epoch_target_id WHERE et.session_id='$rr' AND ws.lifecycle_status='running';" 2>/dev/null)
    prod="$prod $nm:${w}w/ep${e}"
    # drain detection: run-loop alive but no workers AND no active claims for 2+
    # cycles usually means the lane's board filter has no candidates left ->
    # time to rescope (relaunch with FUZZY_MAX=87.999 to take the partial band).
    cl=$(sqlite3 "$DB" "SELECT count(*) FROM target_claims WHERE session_id='$rr' AND status='active';" 2>/dev/null)
    df="/tmp/grind/drained_$nm"
    if [ "${w:-0}" = "0" ] && [ "${cl:-0}" = "0" ] && pgrep -f "run-loop --run-id $rr" >/dev/null 2>&1; then
      n=$(( $(cat "$df" 2>/dev/null || echo 0) + 1 )); echo "$n" > "$df"
      if [ "$n" -ge 2 ]; then echo "[$ts] RESCOPE-HINT $nm: no workers+no claims ${n} cycles — fuzzy-0 band likely drained; relaunch lane with FUZZY_MAX=87.999 (see memory: fs-lane rescope runbook)" >> "$LOG"; fi
    else
      rm -f "$df"
    fi
  done
  echo "[$ts] PROD$prod" >> "$LOG"

  # 2. A tmux name alone is not health: paused placeholders previously made
  # the dashboard claim all three lanes were alive with zero workers. Respect
  # the pause sentinel and otherwise require the recorded run-loop process.
  if fleet_is_paused; then
    paused=1
    echo "[$ts] HARNESS-PAUSED (lane restart and publication suppressed)" >> "$LOG"
  else
    for lane in \
      "small|colo-fs-small|$small_run" \
      "medium|colo-fs-medium|$medium_run" \
      "large|colo-fs-large|$large_run"; do
      lane_name=${lane%%|*}; rest=${lane#*|}; session=${rest%%|*}; rid=${rest#*|}
      if fleet_is_paused; then
        paused=1
        break
      fi
      if [ -z "$rid" ]; then
        echo "[$ts] SKIP $session (no valid persisted run ID)" >> "$LOG"
        continue
      fi
      if ! fleet_lane_alive "$session" "$rid"; then
        if ! command=$(fleet_lane_restart_command "$lane_name" "$rid" "$RUNTIME_PATH"); then
          echo "[$ts] SKIP $session (invalid restart override)" >> "$LOG"
          continue
        fi
        tmux kill-session -t "$session" 2>/dev/null || true
        tmux new-session -d -s "$session" "$command"
        restarted_lanes="$restarted_lanes $session"
        echo "[$ts] RESTARTED $session (missing real run-loop)" >> "$LOG"
      fi
    done
  fi
  if ! fleet_is_paused; then
    tmux has-session -t harness-dashboard 2>/dev/null || { tmux new-session -d -s harness-dashboard "exec env PATH=$RUNTIME_PATH bash -c 'cd $HARNESS && bun run ui:server 2>&1 | tee /tmp/grind/dashboard.log'"; echo "[$ts] RESTARTED harness-dashboard" >> "$LOG"; }
    tmux has-session -t crack-watch 2>/dev/null || { tmux new-session -d -s crack-watch "exec env PATH=$RUNTIME_PATH bash $FLEET/crack_watch.sh"; echo "[$ts] RESTARTED crack-watch" >> "$LOG"; }
  else
    paused=1
  fi
  # 3. restart the publisher with the same patched fleet directory as this watchdog.
  if ! fleet_is_paused; then
    pgrep -f "${FS_PUSH_SCRIPT}" >/dev/null 2>&1 || {
      nohup bash "$FS_PUSH_SCRIPT" > /tmp/grind/fs_push.log 2>&1 & disown
      echo "[$ts] RESTARTED loop fs_push" >> "$LOG"
    }
  else
    paused=1
  fi
  if fleet_is_paused; then
    paused=1
    echo "[$ts] ok reaped=$reaped (paused; state recovery and GC skipped)" >> "$LOG"
    [ "${WATCHDOG_ONCE:-0}" = 1 ] && break
    sleep 480
    continue
  fi
  # 3b. stall recovery: a lane whose babysit is alive but produced no session in
  #     55 min (past any rung budget incl sol) is hung on a stuck provider call ->
  #     restart it (babysit startup recovery reclaims its targets).
  for pair in "colo-fs-small:$small_run" "colo-fs-medium:$medium_run" "colo-fs-large:$large_run"; do
    if fleet_is_paused; then
      paused=1
      break
    fi
    ls=${pair%%:*}; rid=${pair#*:}
    [ -z "$rid" ] && continue
    [[ "$rid" == PAUSED-* ]] && continue
    tmux has-session -t "$ls" 2>/dev/null || continue
    case " $restarted_lanes " in *" $ls "*) continue ;; esac
    fleet_tmux_session_younger_than "$ls" 300 && continue
    active_workers=$(sqlite3 "$DB" "SELECT count(*) FROM worker_state WHERE session_id='$rid' AND lifecycle_status='running';" 2>/dev/null)
    active_claims=$(sqlite3 "$DB" "SELECT count(*) FROM target_claims WHERE session_id='$rid' AND status='active';" 2>/dev/null)
    case "$active_workers" in ""|*[!0-9]*) continue ;; esac
    case "$active_claims" in ""|*[!0-9]*) continue ;; esac
    [ "$active_workers" = 0 ] && [ "$active_claims" = 0 ] && continue
    mins=$(sqlite3 "$DB" "SELECT CAST((julianday('now')-julianday(max(ts)))*1440 AS INT) FROM (SELECT created_at AS ts FROM pi_sessions WHERE run_id='$rid' UNION ALL SELECT started_at AS ts FROM worker_state WHERE session_id='$rid');" 2>/dev/null)
    if [ -n "$mins" ] && [ "$mins" -gt 55 ] 2>/dev/null; then
      if fleet_is_paused; then
        paused=1
        break
      fi
      case "$ls" in
        colo-fs-small) lane_name=small ;;
        colo-fs-medium) lane_name=medium ;;
        colo-fs-large) lane_name=large ;;
        *) continue ;;
      esac
      if ! command=$(fleet_lane_restart_command "$lane_name" "$rid" "$RUNTIME_PATH"); then
        echo "[$ts] SKIP stalled $ls (invalid restart override)" >> "$LOG"
        continue
      fi
      tmux kill-session -t "$ls" 2>/dev/null; pkill -f "$rid" 2>/dev/null; sleep 2
      tmux new-session -d -s "$ls" "$command"
      echo "[$ts] RECOVERED stalled lane $ls (no session ${mins}min)" >> "$LOG"
    fi
  done
  if fleet_is_paused; then
    echo "[$ts] HARNESS-PAUSED mid-cycle (remaining state recovery and GC skipped)" >> "$LOG"
    echo "[$ts] ok reaped=$reaped (paused; state recovery and GC skipped)" >> "$LOG"
    [ "${WATCHDOG_ONCE:-0}" = 1 ] && break
    sleep 480
    continue
  fi
  # 3c. phantom-claim recovery: a live run-loop can retain an active claim after
  #     its child worker died. Match the real worker command, not the run-loop
  #     PID embedded in worker_id, or the missing child is invisible forever.
  for pair in "sm:/Users/douglaswhittingham/pkmn-colosseum-fromscratch:$small_run" \
              "md:/Users/douglaswhittingham/pkmn-colosseum-fs-medium:$medium_run" \
              "lg:/Users/douglaswhittingham/pkmn-colosseum-fs-large:$large_run"; do
    if fleet_is_paused; then
      paused=1
      break
    fi
    nm=${pair%%:*}; rest=${pair#*:}; repo=${rest%%:*}; rid=${rest##*:}
    [ -z "$rid" ] && continue
    [[ "$rid" == PAUSED-* ]] && continue
    while IFS='|' read -r claim_id wid worker_path claim_age; do
      [ -n "$claim_id" ] || continue
      [ -n "$wid" ] || continue
      case "$claim_age" in ""|*[!0-9]*) continue ;; esac
      [ "$claim_age" -ge 120 ] || continue
      pgrep -f "worker --run-id $rid --worker-id $wid" >/dev/null 2>&1 && continue
      if fleet_is_paused; then
        paused=1
        break
      fi
      quarantined=$(fleet_quarantine_broken_worktree "$worker_path" "$claim_id" 2>/dev/null || true)
      if [ -n "$quarantined" ]; then
        echo "[$ts] QUARANTINED broken worktree $nm $wid -> $quarantined" >> "$LOG"
      fi
      if ORCH_AGENT_KERNEL_DATABASE_URL="postgres://agent_kernel:agent_kernel@127.0.0.1:55432/agent_kernel" ORCH_AGENT_KERNEL_REQUIRED=1 \
          bun apps/server/src/job-runner.ts --project pkmn-colosseum --repo-root "$repo" recover-claims --run-id "$rid" --worker-id "$wid" --reason "watchdog: active claim has no worker process" --force >> /tmp/grind/watchdog_recover.log 2>&1; then
        echo "[$ts] RECOVERED phantom claim $nm $wid" >> "$LOG"
      else
        echo "[$ts] ERROR failed to recover phantom claim $nm $wid" >> "$LOG"
      fi
    done < <(sqlite3 -separator '|' "$DB" "SELECT c.id, c.worker_id, COALESCE(c.worktree_path,''), CAST((julianday('now')-julianday(c.claimed_at))*86400 AS INT) FROM target_claims c WHERE c.session_id='$rid' AND c.status='active' AND c.worker_id LIKE 'runloop-%';" 2>/dev/null)
  done

  if fleet_is_paused; then
    echo "[$ts] HARNESS-PAUSED mid-cycle (strike ingestion and GC skipped)" >> "$LOG"
    echo "[$ts] ok reaped=$reaped (paused; state recovery and GC skipped)" >> "$LOG"
    [ "${WATCHDOG_ONCE:-0}" = 1 ] && break
    sleep 480
    continue
  fi

  # 3d. ingest strike notes into KG path_facts (idempotent)
  python3 "$FLEET/strike_notes_ingest.py" >> /tmp/grind/strike_ingest.log 2>&1

  if fleet_is_paused; then
    echo "[$ts] HARNESS-PAUSED mid-cycle (GC skipped)" >> "$LOG"
    echo "[$ts] ok reaped=$reaped (paused; GC skipped)" >> "$LOG"
    [ "${WATCHDOG_ONCE:-0}" = 1 ] && break
    sleep 480
    continue
  fi

  # 4. Remove only registered, stale, non-active worker worktrees. A failed DB
  # read fails closed; it must never turn an empty active list into mass removal.
  active_paths=/tmp/grind/active_wt_paths.txt
  if sqlite3 "$DB" "SELECT DISTINCT worktree_path FROM worker_state WHERE lifecycle_status='running' AND worktree_path IS NOT NULL;" > "${active_paths}.tmp" 2>/dev/null; then
    mv "${active_paths}.tmp" "$active_paths"
    while IFS= read -r worktree_root; do
      for d in "$worktree_root"/*/; do
        fleet_is_paused && break
        [ -d "$d" ] || continue
        source_path="${d%/}/source"
        claim_id=${d%/}; claim_id=${claim_id##*/}
        if ! fleet_managed_worker_path "$source_path" "$claim_id" >/dev/null; then
          echo "[$ts] GC skipped unmanaged path $source_path" >> "$LOG"
          continue
        fi
        grep -Fxq "$source_path" "$active_paths" 2>/dev/null && continue
        if [ -z "$(find "$d" -maxdepth 0 -mmin -90 2>/dev/null)" ]; then
          if git -C "$GAME" worktree remove --force "$source_path" 2>/dev/null; then
            echo "[$ts] GC removed $source_path" >> "$LOG"
          else
            echo "[$ts] GC skipped unregistered/failed $source_path" >> "$LOG"
          fi
        fi
      done
      fleet_is_paused && break
    done < <(fleet_worker_worktree_roots)
  else
    rm -f "${active_paths}.tmp"
    echo "[$ts] GC skipped: failed to read active worktrees" >> "$LOG"
  fi
  if fleet_is_paused; then
    echo "[$ts] HARNESS-PAUSED mid-cycle (remaining GC skipped)" >> "$LOG"
    echo "[$ts] ok reaped=$reaped (paused; GC interrupted safely)" >> "$LOG"
    [ "${WATCHDOG_ONCE:-0}" = 1 ] && break
    sleep 480
    continue
  fi
  if ! fleet_is_paused; then git -C "$HARNESS" worktree prune 2>/dev/null; fi
  if ! fleet_is_paused; then git -C "$GAME" worktree prune 2>/dev/null; fi
  if ! fleet_is_paused; then git -C /Users/douglaswhittingham/pkmn-colosseum-fromscratch worktree prune 2>/dev/null; fi
  if ! fleet_is_paused; then git -C /Users/douglaswhittingham/pkmn-colosseum-fs-medium worktree prune 2>/dev/null; fi
  if ! fleet_is_paused; then git -C /Users/douglaswhittingham/pkmn-colosseum-fs-large worktree prune 2>/dev/null; fi
  wt=$(ls -d "$HARNESS"/projects/pkmn-colosseum/worktrees/*/ 2>/dev/null | wc -l | tr -d ' ')
  disk=$(df -h "$HARNESS" 2>/dev/null | tail -1 | awk '{print $5}')
  echo "[$ts] ok reaped=$reaped worktrees=$wt disk=$disk" >> "$LOG"
  [ "${WATCHDOG_ONCE:-0}" = 1 ] && break
  sleep 480
done
