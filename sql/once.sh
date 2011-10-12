#!/bin/bash
source conf.sh
function update_object()
{
    echo "update_object"
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111010_2.sql
    . clearshiyamen.sh
}

function update_data()
{
    echo "update_data"
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from loot; source loot;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from map_object; source mapobject;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from item_template; source itemtemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from npc_group; source npcgroup;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon_level; source dungeonlevel;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon_monster; source dungeonmonster;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon; source dungeon;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from acupra; source acupra;";
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
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from clan_tech_template; source clantech;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from clan_lvl; source clanlvl;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from fighter_prob; source fighterprob;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from copy; source copy;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from map; source map;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from map_spot; source mapspot;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from equipment_set; source equipmentset;";
}

update_data
update_object
