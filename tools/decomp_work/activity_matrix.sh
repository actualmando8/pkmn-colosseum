#!/bin/bash
# activity_matrix.sh — live "matrix" feed of RE-agent activity: functions being worked,
# worker output, and git commits as they land. Runs in pane %7 (replaces dead qwen-3090).
REPO=/mnt/c/Users/douglaswhittingham/pkmn-colosseum
cd "$REPO" 2>/dev/null || exit 1
G=$'\e[38;5;46m'; C=$'\e[38;5;51m'; Y=$'\e[38;5;226m'; M=$'\e[38;5;201m'; D=$'\e[38;5;240m'; R=$'\e[0m'
last_commit=""
echo "${G}=== RE-AGENT ACTIVITY MATRIX ===${R}"
while true; do
  ts=$(date +%H:%M:%S)
  # new git commits (decomp wins + pcport)
  cur=$(git log --oneline -1 2>/dev/null | awk '{print $1}')
  if [ -n "$cur" ] && [ "$cur" != "$last_commit" ]; then
    [ -n "$last_commit" ] && echo "${M}${ts} COMMIT ${R}$(git log --oneline -1 2>/dev/null | cut -c1-70)"
    last_commit=$cur
  fi
  # latest worker activity (deepseek + qwen logs + permuter swarm + research log)
  ds=$(tail -1 tools/decomp_work/overnight/logs/deepseek_v4_live.log 2>/dev/null | grep -oE "START fn_[0-9A-F]+|REJECT fn_[0-9A-F]+.*|[0-9.]+% via" | head -1)
  [ -n "$ds" ] && echo "${C}${ts} DEEPSEEK ${R}$ds"
  rl=$(tail -1 .omc/research_log.jsonl 2>/dev/null | python3 -c "import sys,json
try:
 d=json.loads(sys.stdin.read()); print(d.get('stage','?'),d.get('fn',''),d.get('msg','')[:50])
except: pass" 2>/dev/null)
  [ -n "$rl" ] && echo "${Y}${ts} SWARM    ${R}$rl"
  # active annealer chains
  ann=$(python3 -c "import json
try:
 d=json.load(open('.omc/permuter_state.json')); a=d.get('active',{})
 print(' '.join(f\"{v.get('fn','')}={v.get('score','?')}\" for v in list(a.values())[:4]))
except: pass" 2>/dev/null)
  [ -n "$ann" ] && echo "${D}${ts} ANNEAL   ${ann}${R}"
  sleep 4
done
