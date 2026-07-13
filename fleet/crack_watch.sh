#!/bin/bash
# Appends newly scored/cracked fs-lane targets to /tmp/grind/crack_watch.log.
# Keep this script compatible with the macOS system Bash (3.2).

DB=${CRACK_WATCH_DB:-/Users/douglaswhittingham/gamecube-decomp-harness/projects/pkmn-colosseum/state/orchestrator.sqlite}
LOG=${CRACK_WATCH_LOG:-/tmp/grind/crack_watch.log}
SEEN=${CRACK_WATCH_SEEN:-/tmp/grind/crack_watch.seen}
POLL_SECONDS=${CRACK_WATCH_POLL_SECONDS:-90}
SMALL_RUN_FILE=${CRACK_WATCH_SMALL_RUN_FILE:-/tmp/grind/fs-small_run.txt}
MEDIUM_RUN_FILE=${CRACK_WATCH_MEDIUM_RUN_FILE:-/tmp/grind/fs-medium_run.txt}
LARGE_RUN_FILE=${CRACK_WATCH_LARGE_RUN_FILE:-/tmp/grind/fs-large_run.txt}

active_run_ids() {
  local run_file run_id

  for run_file in "$SMALL_RUN_FILE" "$MEDIUM_RUN_FILE" "$LARGE_RUN_FILE"; do
    [ -r "$run_file" ] || continue
    run_id=$(sed -n '1{s/^[[:space:]]*//;s/[[:space:]]*$//;p;}' "$run_file" 2>/dev/null)

    case "$run_id" in
      ""|PAUSED|PAUSED-*) continue ;;
      *[!A-Za-z0-9._:-]*) continue ;;
    esac

    printf '%s\n' "$run_id"
  done
}

run_id_sql_list() {
  local run_id run_ids sql_list

  run_ids=$(active_run_ids)
  [ -n "$run_ids" ] || return 1

  sql_list=
  while IFS= read -r run_id; do
    [ -n "$run_id" ] || continue
    if [ -n "$sql_list" ]; then
      sql_list="$sql_list,"
    fi
    sql_list="${sql_list}'${run_id}'"
  done <<EOF
$run_ids
EOF

  [ -n "$sql_list" ] || return 1
  printf '%s\n' "$sql_list"
}

score_query() {
  local lanes

  lanes=$(run_id_sql_list) || return 1
  printf '%s\n' "SELECT substr(session_id,1,8)||' '||target_key||' score='||round(best_score,2)||' exact='||exact FROM worker_state WHERE session_id IN ($lanes) AND (exact=1 OR best_score>0)"
}

current_scores() {
  local query

  query=$(score_query) || return 0
  sqlite3 "$DB" "$query" 2>/dev/null | sort
}

poll_scores() {
  local now new

  now=$(current_scores)
  [ -n "$now" ] || return 0

  new=$(printf '%s\n' "$now" | comm -13 "$SEEN" -)
  if [ -n "$new" ]; then
    printf '%s\n' "$new" | while IFS= read -r line; do
      printf '[%s] SCORED: %s\n' "$(date +%H:%M)" "$line" >> "$LOG"
    done
    printf '%s\n' "$now" > "$SEEN"
  fi
}

crack_watch_main() {
  current_scores > "$SEEN"
  while true; do
    sleep "$POLL_SECONDS"
    poll_scores
  done
}

if [ "${BASH_SOURCE[0]}" = "$0" ]; then
  crack_watch_main "$@"
fi
