#!/bin/sh
#export LD_LIBRARY_PATH=/opt/app/mysql/lib/mysql/:/opt/app/libevent/lib
#echo "1" > /proc/sys/kernel/core_uses_pid
#ulimit -n 16384 -c unlimited
while true
do
#  valgrind --tool=memcheck --leak-check=full bin/Debug/Server.ASSS 2>`date +memcheck_%y%m%d_%H%M%S`
  bin/Debug/Server.ASSS
  sleep 1
done
