#!/bin/bash
F=/storage/finetune/pkmn-colosseum-2026/farm
# stop any existing daemon by pid (avoid pattern self-match)
for p in $(ps -eo pid,cmd | grep supervisor_daemon | grep -v grep | awk '{print $1}'); do
  [ "$p" = "$$" ] || kill "$p" 2>/dev/null
done
sleep 1
setsid nohup nice -n 5 bash "$F/supervisor_daemon.sh" </dev/null >/dev/null 2>&1 &
disown 2>/dev/null
sleep 2
c=$(ps -eo pid,cmd | grep supervisor_daemon | grep -v grep | grep -vc " $$ ")
echo "daemon procs: $c"
