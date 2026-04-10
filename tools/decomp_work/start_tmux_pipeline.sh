#!/bin/bash
# Run THIS from your tmux terminal to create the decomp pipeline layout
# Usage: bash tools/decomp_work/start_tmux_pipeline.sh

TMUX_BIN="${LOCALAPPDATA}/Microsoft/WinGet/Packages/arndawg.tmux-windows_Microsoft.Winget.Source_8wekyb3d8bbwe/tmux.exe"
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"

echo "Starting decomp pipeline in tmux main-horizontal layout..."
echo "Root: $ROOT"

# Kill old session
"$TMUX_BIN" kill-session -t decomp 2>/dev/null

# Create session
"$TMUX_BIN" new-session -d -s decomp -n pipeline -x 220 -y 60

# Pane 0 (top): Status monitor
"$TMUX_BIN" send-keys -t decomp:pipeline.0 \
  "cd '$ROOT' && while true; do clear; echo '=== DECOMP PIPELINE STATUS ==='; python tools/decomp_scheduler.py --status 2>/dev/null; echo; echo \"Candidates: \$(ls tools/decomp_work/candidates/*.json 2>/dev/null | wc -l) | Review: \$(ls tools/decomp_work/review/*.json 2>/dev/null | wc -l) | Applied: \$(ls tools/decomp_work/applied/*.json 2>/dev/null | wc -l)\"; sleep 8; done" Enter

# Split bottom left: Lane 1 - Ollama/Kimi generator
"$TMUX_BIN" split-window -t decomp:pipeline -v -p 70
"$TMUX_BIN" send-keys -t decomp:pipeline.1 \
  "cd '$ROOT' && echo '=== LANE 1: OLLAMA + KIMI ===' && python tools/decomp_work/lane_ollama.py 2>&1 | tee tools/decomp_work/lane1.log" Enter

# Split bottom center: Lane 2 - Verify/Apply
"$TMUX_BIN" split-window -t decomp:pipeline.1 -h -p 66
"$TMUX_BIN" send-keys -t decomp:pipeline.2 \
  "cd '$ROOT' && echo '=== LANE 2: VERIFY + APPLY ===' && python tools/decomp_work/lane_verify.py 2>&1 | tee tools/decomp_work/lane2.log" Enter

# Split bottom right: Review queue watcher
"$TMUX_BIN" split-window -t decomp:pipeline.2 -h -p 50
"$TMUX_BIN" send-keys -t decomp:pipeline.3 \
  "cd '$ROOT' && while true; do clear; echo '=== REVIEW QUEUE (need fixes) ==='; for f in tools/decomp_work/review/*.json; do [ -f \"\$f\" ] && python -c \"import json; d=json.load(open('\$f')); pct=d.get('status','?').split('_')[-1] if 'partial' in d.get('status','') else '?'; print(f'  {pct:>3s}%  {d[\\\"function\\\"]:20s}  {d[\\\"file\\\"]}')\" 2>/dev/null; done | sort -rn; echo; echo \"Applied: \$(ls tools/decomp_work/applied/*.json 2>/dev/null | wc -l)\"; sleep 6; done" Enter

# Set main-horizontal layout
"$TMUX_BIN" select-layout -t decomp:pipeline main-horizontal

echo "Done! Attach with:"
echo "  $TMUX_BIN attach -t decomp"
