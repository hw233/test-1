#!/bin/bash

source conf.sh

isnum=`echo $1 | egrep -c "^([0-9][0-9]*|[0-9][0-9]*\.[0-9][0-9]*)$"`
if [ $isnum -eq 0 ]; then
    echo "jump $1 . continue..."
    exit -1
fi

function dorm()
{
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from activityData where playerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from auto_battle where playerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from player where id = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from equipment where id in (select id from item where ownerId = $1)";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from item where ownerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from fighter where playerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from fighter_buff where playerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from fighter_train where ownerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from friend where id = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from task_instance where ownerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from tripod where id = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from mail where playerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from var where playerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from player_buff where id = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from practice_data where id = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from practice_place where ownerid = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from player_copy where playerId = $1";
    mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from player_frontmap where playerId = $1";
}

echo "removing $1 ..."
dorm $1
