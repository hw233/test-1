#!/bin/bash
source conf.sh

function update_data()
{
    echo "update_data"
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111109_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111110_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111111_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111129_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111211_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111213_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111219_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20111228_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120104_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120227_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120301_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120317_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120411_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120717_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120814_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20120903_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20121019_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20121023_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20121119_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130110_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20121221_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130127_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130227_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130315_1.sql
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
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from team_copy; source teamcopy;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from towndeamon_monster; source towndeamonmonster;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from eupgrade; source eupgrade;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from area; source area;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from spirit_attr; source spiritattr;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from soul_item_exp; source soulitemexp;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from soul_lvl_exp; source soullvlexp;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from soul_skill_template; source soulskilltemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skillstrengthen; source skillstrengthen;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skillstrengthenprob; source skillstrengthenprob;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skillstrengthen_skill; source skillstrengthen_skill;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skillstrengthen_effect; source skillstrengthen_effect;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from clan_copy_template; source clancopytemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from clan_copy_monster_template; source clancopymonstertemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from clan_statue_template; source clanstatuetemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from lbskills; source lbskills;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dreamer_template; source dreamer_template;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_pinjie; source pet_pinjie;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_gengu; source pet_gengu;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_pressure; source pet_pressure;";
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
