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

tar cvfz ../release/$file bin/Release/Server.ASSS.symbol bin/Release/Server.ASSS scripts/ sql/acupra sql/cittaeffect sql/clanskill sql/dungeonlevel sql/fighterprob sql/itemtemplate sql/mapobject sql/equipmentset sql/npc sql/skills sql/cittas sql/clantech sql/copy sql/dungeonmonster sql/formation sql/loot sql/map sql/attrextra sql/clanlvl sql/dungeon sql/frontmap sql/lvlexp sql/mapspot sql/npcgroup sql/skilleffect sql/data.sh sql/object.sh  sql/updates sql/Object.sql sql/ObjectInsert.sql sql/Data.sql sql/DataInsert.sql sql/once.sh sql/all.sh sql/new.sh sql/newall.sh sql/clear.sh conf/udplog.xml sql/makelog.sh sql/DataLog.sql sql/log_conf.sh sql/DataLogUpdate.sql sql/clearshimen.sh  sql/clearshiyamen.sh  sql/clearyamen.sh

num=`expr $num + 1`
echo $num > .num
