#!/usr/bin/env bash
# Overnight autonomy watchdog (rebuilt 2026-07-10 after host crash).
# Every 8 min: reap hung kernel sessions, restart dead lanes/loops, recover
# phantom claims, prune stale worktrees, log a productivity trail.
HARNESS=/Users/douglaswhittingham/gamecube-decomp-harness
GAME=/Users/douglaswhittingham/pkmn-colosseum
DB="$HARNESS/projects/pkmn-colosseum/state/orchestrator.sqlite"
LOG=/tmp/grind/watchdog.log
declare -A LOOPS=(
  [fs_push]="$GAME/fleet/fs_push.sh"
)
cd "$HARNESS"
mkdir -p /tmp/grind
[ -f /tmp/grind/fs-small_run.txt ] || echo db660bd8-9e0a-4052-b04c-3572b0a62116 > /tmp/grind/fs-small_run.txt
[ -f /tmp/grind/fs-medium_run.txt ] || echo 88c578a7-6e26-40cd-aaa7-f7449d9f7e2a > /tmp/grind/fs-medium_run.txt
[ -f /tmp/grind/fs-large_run.txt ] || echo 5f317c77-c320-430f-96cb-c14b537ccf71 > /tmp/grind/fs-large_run.txt
while true; do
  ts=$(date '+%m-%d %H:%M')
  # 1. reap hung kernel sessions (>45 min = past any rung budget) -> frees worker slots
  reaped=$(docker exec agent-kernel-db psql -U agent_kernel -d agent_kernel -tAc \
    "WITH x AS (UPDATE pi_agent_sessions SET status='error' WHERE status='running' AND created_at < now() - interval '45 minutes' RETURNING 1) SELECT count(*) FROM x;" 2>/dev/null | tr -d ' ')
  # 1c. codex-spark specific: reap sessions >20min (its budget) so it never wedges a slot.
  docker exec agent-kernel-db psql -U agent_kernel -d agent_kernel -tAc "UPDATE pi_agent_sessions SET status='error' WHERE status='running' AND model='gpt-5.3-codex-spark' AND created_at < now() - interval '20 minutes';" >/dev/null 2>&1
  # productivity trail: finished/size per lane
  prod=""
  for pr in "sm:$(cat /tmp/grind/fs-small_run.txt 2>/dev/null)" "md:$(cat /tmp/grind/fs-medium_run.txt 2>/dev/null)" "lg:$(cat /tmp/grind/fs-large_run.txt 2>/dev/null)"; do
    nm=${pr%%:*}; rr=${pr#*:}; [ -z "$rr" ] && continue
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

  # 2. restart dead lane tmux
  tmux has-session -t colo-fs-small 2>/dev/null || { tmux new-session -d -s colo-fs-small "MAXW=5 bash projects/pkmn-colosseum/ops/start-fs-small.sh"; echo "[$ts] RESTARTED colo-fs-small" >> "$LOG"; }
  tmux has-session -t colo-fs-medium 2>/dev/null || { tmux new-session -d -s colo-fs-medium "MAXW=2 bash projects/pkmn-colosseum/ops/start-fs-medium.sh"; echo "[$ts] RESTARTED colo-fs-medium" >> "$LOG"; }
  tmux has-session -t colo-fs-large 2>/dev/null || { tmux new-session -d -s colo-fs-large "MAXW=1 bash projects/pkmn-colosseum/ops/start-fs-large.sh"; echo "[$ts] RESTARTED colo-fs-large" >> "$LOG"; }
  tmux has-session -t harness-dashboard 2>/dev/null || { tmux new-session -d -s harness-dashboard "bun run ui:server 2>&1 | tee /tmp/grind/dashboard.log"; echo "[$ts] RESTARTED harness-dashboard" >> "$LOG"; }
  tmux has-session -t crack-watch 2>/dev/null || { tmux new-session -d -s crack-watch "bash $GAME/fleet/crack_watch.sh"; echo "[$ts] RESTARTED crack-watch" >> "$LOG"; }
  # 3. restart dead background loops
  for name in "${!LOOPS[@]}"; do
    pgrep -f "${LOOPS[$name]##*/}" >/dev/null 2>&1 || { nohup bash "${LOOPS[$name]}" > "/tmp/grind/${name}.log" 2>&1 & disown; echo "[$ts] RESTARTED loop $name" >> "$LOG"; }
  done
  # 3b. stall recovery: a lane whose babysit is alive but produced no session in
  #     55 min (past any rung budget incl sol) is hung on a stuck provider call ->
  #     restart it (babysit startup recovery reclaims its targets).
  for pair in "colo-fs-small:$(cat /tmp/grind/fs-small_run.txt 2>/dev/null)" "colo-fs-medium:$(cat /tmp/grind/fs-medium_run.txt 2>/dev/null)" "colo-fs-large:$(cat /tmp/grind/fs-large_run.txt 2>/dev/null)"; do
    ls=${pair%%:*}; rid=${pair#*:}
    [ -z "$rid" ] && continue
    tmux has-session -t "$ls" 2>/dev/null || continue
    mins=$(sqlite3 "$DB" "SELECT CAST((julianday('now')-julianday(max(created_at)))*1440 AS INT) FROM pi_sessions WHERE run_id='$rid';" 2>/dev/null)
    if [ -n "$mins" ] && [ "$mins" -gt 55 ] 2>/dev/null; then
      tmux kill-session -t "$ls" 2>/dev/null; pkill -f "$rid" 2>/dev/null; sleep 2
      case "$ls" in
        colo-fs-small) tmux new-session -d -s colo-fs-small "MAXW=5 bash projects/pkmn-colosseum/ops/start-fs-small.sh";;
        colo-fs-medium) tmux new-session -d -s colo-fs-medium "MAXW=2 bash projects/pkmn-colosseum/ops/start-fs-medium.sh";;
        colo-fs-large) tmux new-session -d -s colo-fs-large "MAXW=1 bash projects/pkmn-colosseum/ops/start-fs-large.sh";;
      esac
      echo "[$ts] RECOVERED stalled lane $ls (no session ${mins}min)" >> "$LOG"
    fi
  done
  # 3c. phantom-claim recovery: active claims whose owning run-loop PID is dead
  #     block every worker slot and can freeze the epoch boundary (babysit only
  #     auto-recovers at startup and after a mid-run child incident).
  for pair in "sm:/Users/douglaswhittingham/pkmn-colosseum-fromscratch:$(cat /tmp/grind/fs-small_run.txt 2>/dev/null)" \
              "md:/Users/douglaswhittingham/pkmn-colosseum-fs-medium:$(cat /tmp/grind/fs-medium_run.txt 2>/dev/null)" \
              "lg:/Users/douglaswhittingham/pkmn-colosseum-fs-large:$(cat /tmp/grind/fs-large_run.txt 2>/dev/null)"; do
    nm=${pair%%:*}; rest=${pair#*:}; repo=${rest%%:*}; rid=${rest##*:}
    [ -z "$rid" ] && continue
    phantom=0
    while IFS= read -r wid; do
      pid=$(echo "$wid" | cut -d- -f2)
      if [ -n "$pid" ] && ! kill -0 "$pid" 2>/dev/null; then phantom=1; fi
    done < <(sqlite3 "$DB" "SELECT ws.worker_id FROM target_claims c JOIN worker_state ws ON ws.target_claim_id=c.id WHERE c.session_id='$rid' AND c.status='active' AND ws.worker_id LIKE 'runloop-%';" 2>/dev/null)
    if [ "$phantom" = 1 ]; then
      ORCH_AGENT_KERNEL_DATABASE_URL="postgres://agent_kernel:agent_kernel@127.0.0.1:55432/agent_kernel" ORCH_AGENT_KERNEL_REQUIRED=1 \
        bun apps/server/src/job-runner.ts --project pkmn-colosseum --repo-root "$repo" recover-claims --run-id "$rid" --reason "watchdog: phantom claims (owner PID dead)" --force >> /tmp/grind/watchdog_recover.log 2>&1
      echo "[$ts] RECOVERED phantom claims $nm" >> "$LOG"
    fi
  done

  # 3d. ingest strike notes into KG path_facts (idempotent)
  python3 "$GAME/fleet/strike_notes_ingest.py" >> /tmp/grind/strike_ingest.log 2>&1

  # 4. remove STALE worker worktrees (not in active-run use, mtime >90min) + prune.
  sqlite3 "$DB" "SELECT DISTINCT worktree_path FROM worker_state WHERE lifecycle_status='running' AND worktree_path IS NOT NULL;" 2>/dev/null | grep -oE "worktrees/[^/]+" | sort -u > /tmp/grind/active_wt.txt
  for d in "$HARNESS"/projects/pkmn-colosseum/worktrees/*/; do
    id=$(basename "$d")
    grep -q "$id" /tmp/grind/active_wt.txt 2>/dev/null && continue
    [ -z "$(find "$d" -maxdepth 0 -mmin -90 2>/dev/null)" ] && { git -C "$HARNESS" worktree remove --force "$d/source" 2>/dev/null || rm -rf "$d" 2>/dev/null; }
  done
  git -C "$HARNESS" worktree prune 2>/dev/null
  git -C "$GAME" worktree prune 2>/dev/null
  git -C /Users/douglaswhittingham/pkmn-colosseum-fromscratch worktree prune 2>/dev/null
  git -C /Users/douglaswhittingham/pkmn-colosseum-fs-medium worktree prune 2>/dev/null
  git -C /Users/douglaswhittingham/pkmn-colosseum-fs-large worktree prune 2>/dev/null
  wt=$(ls -d "$HARNESS"/projects/pkmn-colosseum/worktrees/*/ 2>/dev/null | wc -l | tr -d ' ')
  disk=$(df -h "$HARNESS" 2>/dev/null | tail -1 | awk '{print $5}')
  echo "[$ts] ok reaped=$reaped worktrees=$wt disk=$disk" >> "$LOG"
  sleep 480
done
