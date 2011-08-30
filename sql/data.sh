#!/bin/bash
source conf.sh

function update()
{
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from loot; source loot;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from map_object; source mapobject;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from item_template; source itemtemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from npc_group; source npcgroup;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon_level; source dungeonlevel;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from acupra; source acupra;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon_monster; source dungeonmonster;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from lvl_exp; source lvlexp;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from special_fighter_template; source npc;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from frontmap; source frontmap;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from attr_extra; source attrextra;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from formation; source formation;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skills; source skills;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skill_effect; source skilleffect;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from cittas; source cittas;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from citta_effect; source cittaeffect;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from clan_skill_template; source clanskill;";
}

selectAction $DBD
if [ $? -eq 1 ]; then
    echo "Updating..."
    update
    echo "Updated!!!"
else
    echo "Cancelled!!!"
fi

