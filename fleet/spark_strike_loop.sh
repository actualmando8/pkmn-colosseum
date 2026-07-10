#!/bin/bash
WT=/Users/douglaswhittingham/spark-strike-wt
LOG=/tmp/grind/spark_strike.log
cd "$WT" || exit 1
TOTAL=$(sed -n '/^<targets>/,/^<\/targets>/p' SOL_TASK.md | grep -c $'\t')
while true; do
  [ -f /tmp/grind/strike_stop ] && { echo "[$(date '+%H:%M')] stop-file — ending" >> "$LOG"; break; }
  DONE=$(grep -c '"symbol"' SOL_NOTES.jsonl 2>/dev/null || echo 0)
  if [ "$DONE" -ge "$TOTAL" ]; then echo "[$(date '+%H:%M')] all $DONE/$TOTAL noted — done" >> "$LOG"; break; fi
  echo "[$(date '+%H:%M')] launching spark session ($DONE/$TOTAL noted)" >> "$LOG"
  codex exec -m gpt-5.3-codex-spark -C "$WT" -s workspace-write --skip-git-repo-check \
    "$(cat SOL_TASK.md)" < /dev/null >> "$LOG" 2>&1
  if tail -5 "$LOG" | grep -qiE 'at capacity|rate limit|usage limit'; then
    echo "[$(date '+%H:%M')] spark limited — retry in 600s" >> "$LOG"; sleep 600
  else
    sleep 20
  fi
done
