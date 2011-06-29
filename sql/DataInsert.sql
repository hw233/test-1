
use data;

source lvlexp

LOCK TABLES `map` WRITE;
INSERT INTO `map` VALUES
(0,'白堰村',1,0,2),
(1,'成都郊外',1,0,2),
(2,'成都',1,0,2),
(3,'慈云寺',1,0,2),
(4,'魏家场',30,0,2),
(5,'桂花山',1,0,2),
(6,'长沙城外',1,0,2),
(7,'长沙',1,0,2),
(8,'青螺山',1,0,2),
(9,'水火风雷魔阵',45,0,2),
(10,'姑婆岭',1,0,2),
(11,'衡山',1,0,2),
(12,'百蛮山',1,0,2),
(13,'莽苍山',1,0,2),
(14,'蜀山',1,0,2),
(15,'都天烈火阵',60,0,2),
(16,'九华山',1,0,2),
(17,'庐山',1,0,2),
(18,'黄山',1,0,2),
(19,'雁荡山',1,0,2),
(20,'东海',1,0,2),
(21,'紫云宫',70,0,2),
(22,'千里黄沙阵',75,0,2),
(23,'罗浮山',1,0,2),
(24,'武当山',1,0,2),
(25,'青城山',1,0,2),
(26,'苏州',1,0,2),
(27,'南疆',1,0,2),
(28,'昆仑山',1,0,2),
(29,'喜马拉雅山',70,0,2),
(30,'依还岭',90,0,2),
(31,'月儿岛',80,0,2),
(32,'陷空岛',1,0,2),
(33,'南疆战场',90,0,2);
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

LOCK TABLES `dungeon_level` WRITE;
source dungeonlevel
UNLOCK TABLES;

-- ASSS
use asss;
LOCK TABLES `practice_place` WRITE;
INSERT INTO `practice_place` VALUES
(1,0,0,100,5,10,1);
UNLOCK TABLES;


