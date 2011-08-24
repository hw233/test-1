#!/bin/bash

now=`date +%Y-%m-%d`

if [ ! -f .date ]; then
    echo `date +%Y-%m-%d` > .date
fi
date=`cat .date`
if [ "$date" != "$now" ]; then
    echo "1" > .num
    date=$now
    echo $date > .date
fi

if [ ! -f .num ]; then
    echo "1" > .num
fi
num=`cat .num`
if [ "$num" == "0" ]; then
    num=1
fi
if [ "$num" == "" ]; then
    num=1
fi

if [ "$1" != "" ]; then
file=$1-$date-$num.tar.gz
else
file=release-$date-$num.tar.gz
fi

tar cvfz ../release/$file bin sql scripts conf run.sh

num=`expr $num + 1`
echo $num > .num
