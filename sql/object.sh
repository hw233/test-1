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
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120214_1.sql
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
