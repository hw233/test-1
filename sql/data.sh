#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from loot; source loot;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from map_object; source mapobject;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from item_template; source itemtemplate;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from npc_group; source npcgroup;";
mysql -h$H -u$U -p$P -P$PT $DBD -e "delete from dungeon_level; source dungeonlevel;";

