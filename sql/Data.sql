-- MySQL dump 10.13  Distrib 5.1.53, for pc-linux-gnu (x86_64)
--
-- Host: localhost    Database: data
-- ------------------------------------------------------
-- Server version	5.1.53-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `area`
--

SET NAMES UTF8;

DROP TABLE IF EXISTS `area`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `area` (
  `id` int(10) unsigned NOT NULL,
  `side` int(1) unsigned NOT NULL DEFAULT '1',
  `area` varchar(255) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `attr_extra`
--

DROP TABLE IF EXISTS `attr_extra`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `attr_extra` (
  `id` int(10) NOT NULL,
  `skill` varchar(255) NOT NULL DEFAULT '',
  `strength` varchar(10) NOT NULL DEFAULT '0',
  `physique` varchar(10) NOT NULL DEFAULT '0',
  `agility` varchar(10) NOT NULL DEFAULT '0',
  `intelligence` varchar(10) NOT NULL DEFAULT '0',
  `will` varchar(10) NOT NULL DEFAULT '0',
  `soul` varchar(10) NOT NULL DEFAULT '0',
  `aura` varchar(10) NOT NULL DEFAULT '0',
  `auraMax` varchar(10) NOT NULL DEFAULT '0',
  `attack` varchar(64) NOT NULL DEFAULT '0',
  `magatk` varchar(64) NOT NULL DEFAULT '0',
  `defend` varchar(64) NOT NULL DEFAULT '0',
  `magdef` varchar(64) NOT NULL DEFAULT '0',
  `hp` varchar(64) NOT NULL DEFAULT '0',
  `tough` varchar(64) NOT NULL DEFAULT '0',
  `action` varchar(64) NOT NULL DEFAULT '0',
  `hitrate` varchar(64) NOT NULL DEFAULT '0',
  `evade` varchar(64) NOT NULL DEFAULT '0',
  `critical` varchar(64) NOT NULL DEFAULT '0',
  `criticaldmg` varchar(64) NOT NULL DEFAULT '0',
  `pierce` varchar(64) NOT NULL DEFAULT '0',
  `counter` varchar(64) NOT NULL DEFAULT '0',
  `magres` varchar(64) NOT NULL DEFAULT '0',
PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `boss`
--

DROP TABLE IF EXISTS `boss`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `boss` (
  `id` int(10) unsigned NOT NULL,
  `location` smallint(5) unsigned NOT NULL,
  `spawnTime` int(10) unsigned NOT NULL,
  `fleeTime` int(10) unsigned NOT NULL,
  `appearLevel` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_hold_monster`
--

DROP TABLE IF EXISTS `clan_hold_monster`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_hold_monster` (
  `id` int(11) NOT NULL,
  `formated` tinyint(3) NOT NULL,
  `monsters` varchar(4095) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clan_hold_monster`
--

LOCK TABLES `clan_hold_monster` WRITE;
/*!40000 ALTER TABLE `clan_hold_monster` DISABLE KEYS */;
INSERT INTO `clan_hold_monster` VALUES (4478,0,'4478,5'),(4479,0,'4479,5'),(4480,0,'4480,6'),(4481,0,'4481,6'),(4482,0,'4482,7'),(4483,0,'4483,7'),(4484,0,'4484,8'),(4485,0,'4485,8'),(4486,0,'4486,9'),(4487,0,'4487,10'),(4488,0,'4488,7'),(4489,0,'4489,7'),(4490,0,'4490,7'),(4491,0,'4491,7'),(4492,0,'4492,7'),(4493,0,'4493,8'),(4494,0,'4494,8'),(4495,0,'4495,8'),(4496,0,'4496,8'),(4497,0,'4497,8'),(4498,0,'4498,9'),(4499,0,'4499,9'),(4500,0,'4500,9'),(4501,0,'4501,9'),(4502,0,'4502,9'),(4503,0,'4503,10'),(4504,0,'4504,10'),(4505,0,'4505,10'),(4506,0,'4506,10'),(4507,0,'4507,10'),(4508,0,'4508,11'),(4509,0,'4509,11'),(4510,0,'4510,11'),(4511,0,'4511,11'),(4512,0,'4512,11'),(4513,0,'4513,12'),(4514,0,'4514,12'),(4515,0,'4515,12'),(4516,0,'4516,12'),(4517,0,'4517,12'),(4518,0,'4518,13'),(4519,0,'4519,13'),(4520,0,'4520,13'),(4521,0,'4521,13'),(4522,0,'4522,13'),(4523,0,'4523,14'),(4524,0,'4524,14'),(4525,0,'4525,14'),(4526,0,'4526,14'),(4527,0,'4527,14'),(4540,1,'4540,4|4529,5,0,4'),(4541,1,'4541,4|4530,6,0,4'),(4542,1,'4542,4|4531,6,0,4'),(4543,1,'4543,4|4532,7,0,4'),(4544,1,'4544,4|4533,7,0,4'),(4545,1,'4545,4|4534,8,0,4'),(4546,1,'4546,4|4535,8,0,4'),(4547,1,'4547,4|4536,9,0,4'),(4548,1,'4548,4|4537,9,0,4'),(4549,1,'4549,4|4538,10,0,4'),(4550,1,'4550,4|4539,10,0,4'),(4562,1,'4562,4|4551,5,0,4'),(4563,1,'4563,4|4552,6,0,4'),(4564,1,'4564,4|4553,6,0,4'),(4565,1,'4565,4|4554,7,0,4'),(4566,1,'4566,4|4555,7,0,4'),(4567,1,'4567,4|4556,8,0,4'),(4568,1,'4568,4|4557,8,0,4'),(4569,1,'4569,4|4558,9,0,4'),(4570,1,'4570,4|4559,9,0,4'),(4571,1,'4571,4|4560,10,0,4'),(4572,1,'4572,4|4561,10,0,4');
/*!40000 ALTER TABLE `clan_hold_monster` ENABLE KEYS */;
UNLOCK TABLES;

DROP TABLE IF EXISTS `clan_lvl`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_lvl` (
  `lvl` tinyint(3) unsigned NOT NULL,
  `exp` bigint(20) unsigned NOT NULL,
  `pkgsize` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weal` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`lvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;

DROP TABLE IF EXISTS `clan_skill_template`;
CREATE TABLE `clan_skill_template` (
  `id` smallint(6) NOT NULL,
  `name` varchar(255) NOT NULL,
  `level` smallint(6) NOT NULL,
  `needs` int(11) NOT NULL,
  `hp` int(11) NOT NULL,
  `attack` int(11) NOT NULL,
  `defend` int(11) NOT NULL,
  `magatk` int(11) NOT NULL,
  `magdef` int(11) NOT NULL,
  PRIMARY KEY (`id`,`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `clan_tech_template`
--

DROP TABLE IF EXISTS `clan_tech_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_tech_template` (
  `id` smallint(6) NOT NULL,
  `level` smallint(6) NOT NULL,
  `needs` int(11) NOT NULL,
  `clanLev` smallint(6) NOT NULL,
  `effect1` int(11) NOT NULL,
  `effect2` int(11) NOT NULL,
  PRIMARY KEY (`id`,`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dungeon`
--

DROP TABLE IF EXISTS `dungeon`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dungeon` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `location` mediumint(5) NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `lvlReq` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dungeon_level`
--

DROP TABLE IF EXISTS `dungeon_level`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dungeon_level` (
  `id` int(10) unsigned NOT NULL,
  `level` int(10) unsigned NOT NULL,
  `monsterSet` int(10) unsigned NOT NULL,
  `lootSet` varchar(4095) NOT NULL,
  UNIQUE KEY `id_level` (`id`,`level`),
  KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `dungeon_monster`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dungeon_monster` (
  `id` int(10) unsigned NOT NULL,
  `formated` tinyint(3) unsigned NOT NULL,
  `monsters` varchar(4095) NOT NULL,
  `experience` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `equipment_set`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `equipment_set` (
  `itemId` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `attrId2` smallint(5) unsigned NOT NULL,
  `attrId4` smallint(5) unsigned NOT NULL,
  `attrId6` smallint(5) unsigned NOT NULL,
  `attrId8` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`itemId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `formation`
--

DROP TABLE IF EXISTS `formation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `formation` (
  `id` smallint(5) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `grid1` tinyint(3) NOT NULL,
  `prop1` int(10) NOT NULL,
  `grid2` tinyint(3) NOT NULL,
  `prop2` int(10) NOT NULL,
  `grid3` tinyint(3) NOT NULL,
  `prop3` int(10) NOT NULL,
  `grid4` tinyint(3) NOT NULL,
  `prop4` int(10) NOT NULL,
  `grid5` tinyint(3) NOT NULL,
  `prop5` int(10) NOT NULL,
  `levelup_item` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `item_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_template` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `subClass` tinyint(3) NOT NULL COMMENT '物品类型',
  `career` tinyint(3) NOT NULL COMMENT '职业',
  `reqLev` smallint(6) NOT NULL DEFAULT '1' COMMENT '等级需求',
  `vLev` smallint(6) NOT NULL DEFAULT '1' COMMENT '价值等级',
  `coin` int(10) NOT NULL COMMENT '铜币售价',
  `quality` tinyint(3) NOT NULL COMMENT '品质',
  `maxQuantity` smallint(6) NOT NULL DEFAULT '1' COMMENT '最大堆叠数量',
  `bindType` tinyint(1) NOT NULL COMMENT '绑定类型',
  `energy` smallint(6) unsigned NOT NULL COMMENT '九仪鼎值',
  `trumpExp` smallint(6) NOT NULL COMMENT '法宝经验',
  `data` smallint(6) NOT NULL COMMENT '可使用道具: 作用数值',
  `enchant` smallint(6) NOT NULL COMMENT '附魔类型',
  `attrId` int(10) NOT NULL COMMENT '附加属性',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `loot`
--

DROP TABLE IF EXISTS `loot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `loot` (
  `id` int(10) unsigned NOT NULL,
  `itemId` varchar(4095) NOT NULL DEFAULT '',
  `isPack` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `eupgrade`;
CREATE TABLE `eupgrade` (
 `eqid` int(10) unsigned NOT NULL,
 `stuff` varchar(512) NOT NULL DEFAULT '',
  `targetid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`eqid`)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
--
-- Table structure for table `lvl_exp`
--

DROP TABLE IF EXISTS `lvl_exp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lvl_exp` (
  `lvl` tinyint(3) unsigned NOT NULL,
  `exp` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`lvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `acupra`;
CREATE TABLE `acupra` (
  `id` tinyint(3) unsigned NOT NULL,
  `lvl` tinyint(3) unsigned NOT NULL,
  `needlvl` tinyint(3) unsigned NOT NULL,
  `pra` int(10) unsigned NOT NULL,
  `soulmax` int(10) unsigned NOT NULL,
  `pramax` int(10) unsigned NOT NULL,
  `citslot` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`, `lvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;


--
-- Table structure for table `map`
--

DROP TABLE IF EXISTS `map`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map` (
  `id` tinyint(3) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `acquirable` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `country` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `map_object`
--

DROP TABLE IF EXISTS `map_object`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map_object` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `spot` smallint(5) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `actionType` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `map_spot`
--

DROP TABLE IF EXISTS `map_spot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map_spot` (
  `id` smallint(5) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `type` smallint(3) NOT NULL DEFAULT '0',
  `countryBattle` tinyint(1) NOT NULL DEFAULT '0',
  `x` int(10) NOT NULL DEFAULT '0',
  `y` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `npc_group`
--

DROP TABLE IF EXISTS `npc_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `npc_group` (
  `id` int(10) unsigned NOT NULL,
  `fighterId` varchar(255) NOT NULL DEFAULT '',
  `formationId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) NOT NULL DEFAULT '0',
  `experience` int(10) unsigned NOT NULL DEFAULT '0',
  `lootId` varchar(255) NOT NULL DEFAULT '',
  `lootId1` varchar(255) NOT NULL DEFAULT '',
  `lootId2` varchar(255) NOT NULL DEFAULT '',
  `lootId3` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `talent`;
CREATE TABLE `talent` (
  `id` smallint(5) NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `cls` tinyint(3) NOT NULL DEFAULT '0',
  `quality` tinyint(3) NOT NULL DEFAULT '0',
  `prob` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  `potential` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  `capacity` float(5,2) unsigned NOT NULL DEFAULT '5.00',
  `strength` varchar(10) NOT NULL DEFAULT '0',
  `physique` varchar(10) NOT NULL DEFAULT '0',
  `agility` varchar(10) NOT NULL DEFAULT '0',
  `intelligence` varchar(10) NOT NULL DEFAULT '0',
  `will` varchar(10) NOT NULL DEFAULT '0',
  `soul` varchar(10) NOT NULL DEFAULT '0',
  `aura` varchar(10) NOT NULL DEFAULT '0',
  `auraMax` varchar(10) NOT NULL DEFAULT '0',
  `attack` varchar(64) NOT NULL DEFAULT '0',
  `magatk` varchar(64) NOT NULL DEFAULT '0',
  `defend` varchar(64) NOT NULL DEFAULT '0',
  `magdef` varchar(64) NOT NULL DEFAULT '0',
  `hp` varchar(64) NOT NULL DEFAULT '0',
  `tough` float(10,4) NOT NULL DEFAULT '0.0000',
  `action` float(10,4) NOT NULL DEFAULT '0.0000',
  `hitrate` float(10,4) NOT NULL DEFAULT '0.0000',
  `evade` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical` float(10,4) NOT NULL DEFAULT '0.0000',
  `criticaldmg` float(10,4) NOT NULL DEFAULT '0.0000',
  `pierce` float(10,4) NOT NULL DEFAULT '0.0000',
  `counter` float(10,4) NOT NULL DEFAULT '0.0000',
  `magres` float(10,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `skills`
--

DROP TABLE IF EXISTS `skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skills` (
  `id` smallint(5) unsigned NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `cond` smallint(5) NOT NULL DEFAULT '0',
  `prob` float(10,4) NOT NULL DEFAULT '0.0000',
  `target` tinyint(3) NOT NULL DEFAULT '0',
  `area` tinyint(3) NOT NULL DEFAULT '0',
  `factor` varchar(255) NOT NULL DEFAULT '',
  `last` smallint(5) NOT NULL DEFAULT '0',
  `cd` smallint(5) NOT NULL DEFAULT '0',
  `effectid` smallint(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `skill_effect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skill_effect` (
  `id` smallint(5) NOT NULL,
  `state` tinyint(3) NOT NULL DEFAULT '0',
  `immune` tinyint(3) NOT NULL DEFAULT '0',
  `disperse` tinyint(3) NOT NULL DEFAULT '0',
  `damage` varchar(255) NOT NULL DEFAULT '',
  `adddam` float(10,4) NOT NULL DEFAULT '0.0000',
  `magdam` varchar(255) NOT NULL DEFAULT '',
  `addmag` float(10,4) NOT NULL DEFAULT '0.0000',
  `crrdam` varchar(255) NOT NULL DEFAULT '',
  `addcrr` float(10,4) NOT NULL DEFAULT '0.0000',
  `hp` varchar(255) NOT NULL DEFAULT '',
  `addhp` float(10,4) NOT NULL DEFAULT '0.0000',
  `absorb` varchar(255) NOT NULL DEFAULT '',
  `thorn` varchar(255) NOT NULL DEFAULT '',
  `inj2hp` varchar(255) NOT NULL DEFAULT '',
  `aura` varchar(255) NOT NULL DEFAULT '',
  `atk` varchar(255) NOT NULL DEFAULT '',
  `def` varchar(255) NOT NULL DEFAULT '',
  `magatk` varchar(255) NOT NULL DEFAULT '',
  `magdef` varchar(255) NOT NULL DEFAULT '',
  `tough` float(10,4) NOT NULL DEFAULT '0.0000',
  `action` varchar(255) NOT NULL DEFAULT '',
  `hitrate` float(10,4) NOT NULL DEFAULT '0.0000',
  `evade` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical` float(10,4) NOT NULL DEFAULT '0.0000',
  `pierce` float(10,4) NOT NULL DEFAULT '0.0000',
  `counter` float(10,4) NOT NULL DEFAULT '0.0000',
  `magres` float(10,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `cittas`;
CREATE TABLE `cittas` (
  `id` smallint(5) NOT NULL,
  `type` smallint(5) NOT NULL,
  `name` varchar(255) NOT NULL,
  `pexp` int(10) unsigned NOT NULL DEFAULT '0',
  `needsoul` smallint(5) NOT NULL DEFAULT '0',
  `effectid` smallint(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `citta_effect`;
CREATE TABLE `citta_effect` (
  `id` smallint(5) NOT NULL,
  `skills` varchar(255) NOT NULL DEFAULT '0',
  `strength` varchar(10) NOT NULL DEFAULT '0',
  `physique` varchar(10) NOT NULL DEFAULT '0',
  `agility` varchar(10) NOT NULL DEFAULT '0',
  `intelligence` varchar(10) NOT NULL DEFAULT '0',
  `will` varchar(10) NOT NULL DEFAULT '0',
  `soul` varchar(10) NOT NULL DEFAULT '0',
  `aura` varchar(10) NOT NULL DEFAULT '0',
  `auraMax` varchar(10) NOT NULL DEFAULT '0',
  `attack` varchar(64) NOT NULL DEFAULT '0',
  `magatk` varchar(64) NOT NULL DEFAULT '0',
  `defend` varchar(64) NOT NULL DEFAULT '0',
  `magdef` varchar(64) NOT NULL DEFAULT '0',
  `hp` varchar(64) NOT NULL DEFAULT '0',
  `tough` varchar(64) NOT NULL DEFAULT '0',
  `action` varchar(64) NOT NULL DEFAULT '0',
  `hitrate` varchar(64) NOT NULL DEFAULT '0',
  `evade` varchar(64) NOT NULL DEFAULT '0',
  `critical` varchar(64) NOT NULL DEFAULT '0',
  `criticaldmg` varchar(64) NOT NULL DEFAULT '0',
  `pierce` varchar(64) NOT NULL DEFAULT '0',
  `counter` varchar(64) NOT NULL DEFAULT '0',
  `magres` varchar(64) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `special_fighter_template`
--

DROP TABLE IF EXISTS `special_fighter_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `special_fighter_template` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `sex` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `potential` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  `capacity` float(5,2) unsigned NOT NULL DEFAULT '5.00',
  `skill` varchar(255) NOT NULL DEFAULT '',
  `npc_weapon` int(10) unsigned NOT NULL DEFAULT '0',
  `strength` int(5) NOT NULL DEFAULT '10',
  `physique` int(5) NOT NULL DEFAULT '10',
  `agility` int(5) NOT NULL DEFAULT '10',
  `intelligence` int(5) NOT NULL DEFAULT '10',
  `will` int(5) NOT NULL DEFAULT '10',
  `soul` int(5) NOT NULL DEFAULT '30',
  `aura` int(5) NOT NULL DEFAULT '10',
  `auraMax` int(5) NOT NULL DEFAULT '10',
  `tough` int(5) NOT NULL DEFAULT '10',
  `attack` int(5) NOT NULL DEFAULT '10',
  `magatk` int(5) NOT NULL DEFAULT '10',
  `defend` int(5) NOT NULL DEFAULT '10',
  `magdef` int(5) NOT NULL DEFAULT '10',
  `hp` int(10) NOT NULL DEFAULT '20',
  `action` int(5) NOT NULL DEFAULT '6000',
  `talent` int(10) NOT NULL DEFAULT '0',
  `hitrate` float(10,4) NOT NULL DEFAULT '90.0000',
  `evade` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical` float(10,4) NOT NULL DEFAULT '0.0000',
  `criticaldmg` float(10,4) NOT NULL DEFAULT '0.0000',
  `pierce` float(10,4) NOT NULL DEFAULT '0.0000',
  `counter` float(10,4) NOT NULL DEFAULT '0.0000',
  `magres` float(10,4) NOT NULL DEFAULT '0.0000',
  `extraPos` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `fighter_prob`;
CREATE TABLE `fighter_prob` (
  `id` int(10) unsigned NOT NULL,
  `free` int(5) NOT NULL DEFAULT '0',
  `gold` int(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
--
-- Table structure for table `weapon_def`
--

DROP TABLE IF EXISTS `weapon_def`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weapon_def` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(100) NOT NULL,
  `classReq` int(1) unsigned NOT NULL DEFAULT '0',
  `areaEffect` int(3) unsigned NOT NULL DEFAULT '1',
  `canCounter` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `copy`;
CREATE TABLE `copy` (
  `id` tinyint(3) unsigned NOT NULL,
  `floor` tinyint(3) unsigned NOT NULL,
  `spot` tinyint(3) unsigned NOT NULL,
  `fighterId` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`, `floor`, `spot`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `frontmap`;
CREATE TABLE `frontmap` (
  `id` tinyint(3) unsigned NOT NULL,
  `spot` tinyint(3) unsigned NOT NULL,
  `count` tinyint(3) unsigned NOT NULL,
  `fighterId` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`, `spot`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `team_copy`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `team_copy` (
  `id` tinyint(3) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `location` smallint(5) unsigned NOT NULL,
  `npcgroups` varchar(100) NOT NULL,
  PRIMARY KEY (`id`, `type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;



-- Dump completed
