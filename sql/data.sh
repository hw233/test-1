#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from loot; source loot;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from map_object; source mapobject;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from item_template; source itemtemplate;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from npc_group; source npcgroup;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from dungeon_level; source dungeonlevel;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from acupra; source acupra;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from dungeon_monster; source dungeonmonster;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from lvl_exp; source lvlexp;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from special_fighter_template; source npc;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from frontmap; source frontmap;";

