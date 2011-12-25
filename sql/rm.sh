#!/bin/bash

source conf.sh

ONLINE="2011-11-30"
LVL=30
IDS=ids-`date +%Y-%m-%d`

selectAction $DBO
if [ $? -eq 1 ]; then
    echo "do removing..."
else
    echo "Cancelled!!!"
    exit
fi

mysql -h$H -u$U -p$P -P$PT $DBO -e "select f.playerId from player as p, fighter as f where p.lastOnline <= unix_timestamp(\"$ONLINE\") and p.gold = 0 and p.totalRecharge = 0 and p.id = f.playerId and f.level < $LVL group by f.playerId order by f.level desc;" > $IDS

awk '{ system(". dorm.sh " $1); }' $IDS

