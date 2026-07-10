#!/bin/bash
# Strike loop: relaunch codex on ANY session end (capacity, context-full, clean)
# until every target has a note in SOL_NOTES.jsonl or /tmp/grind/strike_stop exists.
WT=/Users/douglaswhittingham/strike55-wt
LOG=/tmp/grind/strike55.log
MODEL=gpt-5.5
EFFORT=high
cd "$WT" || exit 1
TOTAL=$(sed -n '/^<targets>/,/^<\/targets>/p' SOL_TASK.md | grep -c $'\t')
while true; do
  [ -f /tmp/grind/strike_stop ] && { echo "[$(date '+%H:%M')] stop-file present — ending" >> "$LOG"; break; }
  DONE=$(grep -c '"symbol"' SOL_NOTES.jsonl 2>/dev/null || echo 0)
  if [ "$DONE" -ge "$((TOTAL-1))" ]; then echo "[$(date '+%H:%M')] all $DONE/$TOTAL targets noted — done" >> "$LOG"; break; fi
  echo "[$(date '+%H:%M')] launching session ($DONE/$TOTAL noted)" >> "$LOG"
  codex exec -m "$MODEL" -C "$WT" -s workspace-write --skip-git-repo-check \
    -c model_reasoning_effort="$EFFORT" \
    "$(cat SOL_TASK.md)

<resume_note>
Before starting: read SOL_NOTES.jsonl and 'git diff --stat' — targets already
noted there are DONE; continue from the next unfinished target in the list.
</resume_note>" < /dev/null >> "$LOG" 2>&1
  if tail -5 "$LOG" | grep -q 'at capacity'; then
    echo "[$(date '+%H:%M')] model at capacity — retry in 300s" >> "$LOG"; sleep 300
  else
    sleep 20
  fi
done
