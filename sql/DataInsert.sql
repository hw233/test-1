
use data;

source lvlexp

LOCK TABLES `map` WRITE;
source map
UNLOCK TABLES;

LOCK TABLES `special_fighter_template` WRITE;
source npc
UNLOCK TABLES;

LOCK TABLES `weapon_def` WRITE;
INSERT INTO `weapon_def` VALUES
(1,'剑',3,1,1),
(2,'锤',1,3,1),
(3,'枪',1,4,1),
(4,'弓',2,7,0),
(5,'飞刀',2,5,0),
(6,'鞭',4,2,1),
(7,'军扇',4,6,0),
(10,'全屏',0,10,0),
(11,'怪物1',0,1,1),
(12,'怪物2',0,1,1),
(13,'怪物3',0,1,1),
(14,'怪物4',0,1,1),
(15,'怪物5',0,1,1),
(129,'NPC剑',0,1,1),
(130,'NPC锤',0,3,1),
(131,'NPC枪',0,4,1),
(133,'NPC飞刀',0,5,0),
(134,'NPC鞭',0,2,1),
(135,'NPC军扇',0,6,0),
(146,'全屏锤',0,10,1),
(147,'全屏枪',0,10,1),
(150,'全屏鞭',0,10,1);
UNLOCK TABLES;

LOCK TABLES `attr_extra` WRITE;
source attrextra
UNLOCK TABLES;

LOCK TABLES `item_template` WRITE;
source itemtemplate
UNLOCK TABLES;

LOCK TABLES `npc_group` WRITE;
source npcgroup
UNLOCK TABLES;

LOCK TABLES `map_spot` WRITE;
source mapspot
UNLOCK TABLES;

LOCK TABLES `map_object` WRITE;
source mapobject
UNLOCK TABLES;

LOCK TABLES `loot` WRITE;
source loot
UNLOCK TABLES;

LOCK TABLES `skills` WRITE;
source skills
UNLOCK TABLES;

LOCK TABLES `skill_effect` WRITE;
source skilleffect
UNLOCK TABLES;

LOCK TABLES `cittas` WRITE;
source cittas 
UNLOCK TABLES;

LOCK TABLES `citta_effect` WRITE;
source cittaeffect
UNLOCK TABLES;

LOCK TABLES `area` WRITE;
INSERT INTO `area` VALUES
(0,1,'0,0'),
(1,1,'0,0'),
(2,1,'0,0;1,0;2,0;3,0;4,0;-1,0;-2,0;-3,0;-4,0'),
(3,1,'0,0;0,1;0,2;0,3;0,4;0,-1;0,-2;0,-3;0,-4'),
(4,1,'0,0;1,0;2,0;3,0;4,0;-1,0;-2,0;-3,0;-4,0;0,1;1,1;2,1;3,1;4,1;-1,1;-2,1;-3,1;-4,1;0,2;1,2;2,2;3,2;4,2;-1,2;-2,2;-3,2;-4,2;0,3;1,3;2,3;3,3;4,3;-1,3;-2,3;-3,3;-4,3;0,4;1,4;2,4;3,4;4,4;-1,4;-2,4;-3,4;-4,4'),
(5,1,'0,0;1,1;1,-1;-1,-1;-1;1');
UNLOCK TABLES;

LOCK TABLES `fighter_prob` WRITE;
source fighterprob
UNLOCK TABLES;

LOCK TABLES `acupra` WRITE;
source acupra
UNLOCK TABLES;

LOCK TABLES `clan_skill_template` WRITE;
source clanskill
UNLOCK TABLES;

LOCK TABLES `clan_tech_template` WRITE;
source clantech
UNLOCK TABLES;


LOCK TABLES `dungeon` WRITE;
source dungeon
UNLOCK TABLES;

LOCK TABLES `dungeon_level` WRITE;
source dungeonlevel
UNLOCK TABLES;

LOCK TABLES `dungeon_monster` WRITE;
source dungeonmonster
UNLOCK TABLES;

LOCK TABLES `formation` WRITE;
source formation
UNLOCK TABLES;

LOCK TABLES `copy` WRITE;
source copy
UNLOCK TABLES;

-- ASSS
use asss;
LOCK TABLES `practice_place` WRITE;
INSERT INTO `practice_place` VALUES
(1,0,0,30,0,0,0,1,0,0,0,0),
(2,0,0,30,0,0,0,1,0,0,0,0),
(3,0,0,30,0,0,0,1,0,0,0,0),
(4,0,0,30,0,0,0,1,0,0,0,0),
(5,0,0,30,0,0,0,1,0,0,0,0),
(6,0,0,30,0,0,0,1,0,0,0,0),
(7,0,0,0,0,0,0,1,0,0,0,0);
UNLOCK TABLES;


