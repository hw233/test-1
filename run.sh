#!/bin/sh
#export LD_LIBRARY_PATH=/opt/app/mysql/lib/mysql/:/opt/app/libevent/lib
#echo "1" > /proc/sys/kernel/core_uses_pid
#ulimit -n 16384 -c unlimited
ulimit -S -c unlimited
ID=00001
PORT=8000
while true
do
#  valgrind --tool=memcheck --leak-check=full bin/Debug/Server.ASSS 2>`date +memcheck_%y%m%d_%H%M%S`
    xxx=`netstat -tnlp |grep ":$PORT"|awk '{print $7}'|awk -F"/" '{print $1}'`
    if [ "$xxx" != "" ]; then sleep 3; continue; fi;
    cd sql && if [ -f once.sh ]; then . once.sh; rm -f once.sh; fi && cd -
    if [ -f bin/Release/Server.$ID ]; then mv -f bin/Release/Server.$ID bin/Release/Server.$ID.bak; fi
    if [ -f bin/Release/Server.ASSS ]; then mv -f bin/Release/Server.ASSS bin/Release/Server.$ID; fi
    if [ -f bin/Release/JsonProxy.ASSS ]; then bin/Release/JsonProxy.ASSS; fi
    bin/Release/Server.$ID
    sleep 3
done
