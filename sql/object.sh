#!/bin/bash
source conf.sh

function update_object()
{
    echo "update_object"
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/ClearQQTenpayAward.sql  # XXX: 清除QQ财付通奖励
    mysql -h$H -u$U -p$P -P$PT $DBO < update/Create_ArenaLayout.sql  # 建立文件竞技场排布
    mysql -h$H -u$U -p$P -P$PT $DBO < update/Create_ArenaRobot.sql  # 建立文件竞技场排布
    mysql -h$H -u$U -p$P -P$PT $DBO < update/Create_Arena_Battle.sql  # 建立文件竞技场排布
}

#sh player_jiguan.sh
#sh clanscore.sh
update_object
#selectAction $DBO
#if [ $? -eq 1 ]; then
#    echo "Updating..."
#    update_object
#    echo "Updated!!!"
#else
#    echo "Cancelled!!!"
#fi
#
