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
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130227_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130315_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130517_1.sql   #修改item_template字段subClass类型
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130524_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130609_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130620_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130813_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130823_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130902_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130913_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130922_2.sql    #修改jiguanyu表字段attrValue类型
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130922_3.sql    #修改jiguanshu表,添加字段element, attrValue1, attrValue2, attrValue3, attrValue4
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20130922_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20131001_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20131025_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20131122_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20131126_Gold.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_Create_pet_sevensoullevel.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_Create_pet_sevensoulupgrade.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140101_1.sql      #灵宠组队副本配置
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_modify_lbskills.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140103_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140114_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140114_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140121_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140124_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140214_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_AddXinMo_0317.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_create_skill_ev.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140404_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140420_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_Create_cardSystem.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140407_drinkAttr.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140124_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140214_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_AddXinMo_0317.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_create_skill_ev.sql
    #mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_20140404_1.sql
    mysql -h$H -u$U -p$P -P$PT $DBD < updates/Data_modify_cardInfo.sql
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from loot; source loot;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from map_object; source mapobject;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from item_template; source itemtemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from npc_group; source npcgroup;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon_level; source dungeonlevel;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon_monster; source dungeonmonster;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from dungeon; source dungeon;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from acupra; source acupra;";
    #mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from lvl_exp; source lvlexp;";   #千万别打开,会影响1~370区的经验表(1~370是老经验表，>370和测试区是新经验表)
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
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_levelup; source pet_levelup;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_neidan; source pet_neidan;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from xingchen; source xingchen;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from jiguanshu; source jiguanshu;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from jiguanyu; source jiguanyu;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from tuzhi; source tuzhi;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from signet; source signet;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_sanhun; source pet_sanhun;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from team_arena_skill; source team_arena_skill;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from team_arena_inspire; source team_arena_inspire;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from zhenwei; source zhenwei;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from acupragold; source acupragold;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_sevensoullevel; source pet_qipo_level;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from pet_sevensoulupgrade; source pet_qipo_skill;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from petteamcopy; source petteamcopy;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from ride; source ride;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from ride_upgrade; source ride_upgrade;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from coupleinfo; source coupleinfo;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from couplecopy; source couplecopy;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from clan_building_template; source clanbuildingtemplate;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from ride_cangjian; source ride_cangjian;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from xinmo; source xinmo;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from zhenyuan_extraAttr; source zhenyuan_extraAttr;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from skill_ev; source skill_ev;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from erlking; source erlking;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from cardInfo; source cardInfo;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from cardupgrade; source cardupgrade;";
    mysql -h$H -u$U -p$P -P$PT $DBD -e "SET NAMES UTF8;delete from drinkAttr; source drinkAttr;";
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
