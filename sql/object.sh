#!/bin/bash
source conf.sh
function update_object()
{
    echo "update_object"
    #mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110918_1.sql
    #mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110913_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111010_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111021_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111024_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111031_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2011101101_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111109_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111114_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111115_1.sql
    
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111202_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111207_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111122.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111129_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111214.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111219.sql
    #
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111219_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111110_1.sql
    #
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111217_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111227_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120111_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120116_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120129_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120214_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120214_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120227_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120301_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120307_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120314_1.sql # must be once
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120316_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120317_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120405_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120406_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120409_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120416_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120417_1.sql # must be once reset world boss time
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120423_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120427_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120503_1.sql # must be once -- fb
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120504_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120508_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120508_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120509_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120608_1.sql
    ###mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120515_1.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/towndeamon_player_fix.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120613_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120619_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120622_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120629_1.sql  # must be once for update arena_bet that recieved
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120703_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120706_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120709_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120629_2.sql  #must be once for update new_relation
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120713_1.sql
    ###mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120720_2.sql
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120722_1.sql
    #. clearshiyamen.sh
}

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
