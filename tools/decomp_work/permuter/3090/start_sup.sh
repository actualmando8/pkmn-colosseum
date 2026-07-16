#!/bin/bash
F=/storage/finetune/pkmn-colosseum-2026/farm
# Stop only the exact daemon command. Pattern matching the full process table
# can kill the SSH shell that invoked this script because its command line also
# contains the words "supervisor_daemon.sh".
mapfile -t pids < <(ps -eo pid=,args= | awk -v script="$F/supervisor_daemon.sh" \
  '$1 == "bash" && $2 == script { print $1 }')
if [ "${#pids[@]}" -gt 0 ]; then
  kill "${pids[@]}" 2>/dev/null || true
fi
sleep 1
setsid nohup nice -n 5 bash "$F/supervisor_daemon.sh" </dev/null >/dev/null 2>&1 &
disown 2>/dev/null
sleep 2
c=$(ps -eo args= | awk -v script="$F/supervisor_daemon.sh" \
  '$1 == "bash" && $2 == script { n++ } END { print n + 0 }')
echo "daemon procs: $c"
