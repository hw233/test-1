#!/bin/bash
source conf.sh

function update_data()
{
    echo "update_data"
   # mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20141023_cHorcruxHoldAttr.sql

   # mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from horcruxHoldAttr1; source horcruxHoldAttr1;";
   # mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from horcruxHoldAttr2; source horcruxHoldAttr2;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from item_template; source itemtemplate;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from lvl_exp; source lvlexp;";   #千万别打开,会影响1~370区的经验表( 1~370是老经验表，>370和测试区是新经验表) 
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skillCondition; source skillCondition;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skillScope; source skillScope;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skillEffect; source skillEffect;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from fighter_base; source fighter_base;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from fighter_base_skill; source fighter_base_skill;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skill; source skill;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from map; source map;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from monster; source monster;";
   mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from battleAward; source battleAward;";
}

update_data
#selectAction $DBD
#if [ $? -eq 1 ]; then
#    echo "Updating..."
#    update_data
#    echo "Updated!!!"
#else
#    echo "Cancelled!!!"
#fi
#
