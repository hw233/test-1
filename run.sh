#!/bin/sh
export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64
echo "1" > /proc/sys/kernel/core_uses_pid
ulimit -n 16384 -c unlimited
while true
do
#  valgrind --tool=memcheck --leak-check=full bin/Debug/Server.HoHH 2>`date +memcheck_%y%m%d_%H%M%S`
  bin/Debug/Server.HoHH
  sleep 1
done
