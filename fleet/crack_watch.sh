#!/bin/bash
# Appends newly scored/cracked fs-lane targets to /tmp/grind/crack_watch.log
DB=/Users/douglaswhittingham/gamecube-decomp-harness/projects/pkmn-colosseum/state/orchestrator.sqlite
LOG=/tmp/grind/crack_watch.log
SEEN=/tmp/grind/crack_watch.seen
LANES="'88c578a7-6e26-40cd-aaa7-f7449d9f7e2a','5f317c77-c320-430f-96cb-c14b537ccf71','db660bd8-9e0a-4052-b04c-3572b0a62116'"
Q="SELECT substr(session_id,1,8)||' '||target_key||' score='||round(best_score,2)||' exact='||exact FROM worker_state WHERE session_id IN ($LANES) AND (exact=1 OR best_score>0)"
sqlite3 "$DB" "$Q" 2>/dev/null | sort > "$SEEN"
while true; do
  sleep 90
  NOW=$(sqlite3 "$DB" "$Q" 2>/dev/null | sort)
  [ -z "$NOW" ] && continue
  NEW=$(printf '%s\n' "$NOW" | comm -13 "$SEEN" -)
  if [ -n "$NEW" ]; then
    printf '%s\n' "$NEW" | while IFS= read -r l; do echo "[$(date +%H:%M)] SCORED: $l" >> "$LOG"; done
    printf '%s\n' "$NOW" > "$SEEN"
  fi
done
