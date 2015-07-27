-- MySQL dump 10.13  Distrib 5.1.53, for pc-linux-gnu (x86_64)                                                                    
--
-- Host: localhost    Database: data
-- ------------------------------------------------------
-- Server version   5.1.53-log

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
DROP TABLE IF EXISTS `item_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_template` (
      `id` int(11) NOT NULL,
      `name` varchar(255) NOT NULL,
      `subClass` tinyint(3) unsigned NOT NULL COMMENT '物品类型',
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
      `salePriceUp` float(10,2) NOT NULL COMMENT '交易价格上限',
      PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


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

DROP TABLE IF EXISTS `skillCondition`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skillCondition` (
      `id` int(10) NOT NULL DEFAULT '0',
      `name` varchar(255) NOT NULL,
      `cond` int(10) NOT NULL DEFAULT '0',
      `prob` tinyint(3) unsigned NOT NULL DEFAULT '0',
      `distance` int(10) NOT NULL DEFAULT '0',
      `priority` tinyint(3) unsigned NOT NULL DEFAULT '0',
      PRIMARY KEY (`id`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `skillScope`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skillScope` (
      `id` int(10) NOT NULL DEFAULT '0',
      `name` varchar(255) NOT NULL,
      `area` tinyint(3) unsigned NOT NULL DEFAULT '0',
      `x` int(10) unsigned NOT NULL DEFAULT '0',
      `y` int(10) unsigned NOT NULL DEFAULT '0',
      `radx` int(10) unsigned NOT NULL DEFAULT '0',
      `rady` int(10) unsigned NOT NULL DEFAULT '0',
      PRIMARY KEY (`id`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `skillEffect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skillEffect` (
      `id` int(10) NOT NULL DEFAULT '0',
      `name` varchar(255) NOT NULL,
      `skillType` tinyint(3) unsigned NOT NULL DEFAULT '0',
      `buffId` tinyint(3) unsigned NOT NULL DEFAULT '0',
      `damage` int(10) NOT NULL DEFAULT '0',
      `damageP` float(10,2) NOT NULL COMMENT '伤害百分比',
      `trerapy` int(10) NOT NULL DEFAULT '0',
      `trerapyP` float(10,2) NOT NULL COMMENT '治疗增幅分比',
      `avoidhurt` tinyint(3) unsigned NOT NULL DEFAULT '0',
      PRIMARY KEY (`id`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `fighter_base`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fighter_base` (
      `id` int(10) NOT NULL DEFAULT '0',
      `name` varchar(255) NOT NULL,
      `color` tinyint(3) NOT NULL DEFAULT '0',
      `typeId` tinyint(3) NOT NULL DEFAULT '0',
      `childType` int(10) NOT NULL DEFAULT '0',
      `speed`  int(10) NOT NULL DEFAULT '0',
      `bodySize`  int(10) NOT NULL DEFAULT '0',
      `skills`  varchar(255) NOT NULL,
      `hp` int(10) NOT NULL DEFAULT '0',
      `attack` int(10) NOT NULL DEFAULT '0',
      `defend` int(10) NOT NULL DEFAULT '0',
      `magatk` int(10) NOT NULL DEFAULT '0',
      `magdef` int(10) NOT NULL DEFAULT '0',
      `critical` int(10) NOT NULL DEFAULT '0',
      `criticalDef` int(10) NOT NULL DEFAULT '0',
      `hit` int(10) NOT NULL DEFAULT '0',
      `evade` int(10) NOT NULL DEFAULT '0',
      PRIMARY KEY (`id`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `fighter_base_skill`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fighter_base_skill` (
      `id` int(10) NOT NULL DEFAULT '0',  /*武将ID*/
      `levelLimit` tinyint(3) NOT NULL DEFAULT '0',  /*技能开启等级*/
      `skillId` int(10) NOT NULL DEFAULT '0',
      PRIMARY KEY (`id`,`levelLimit`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `skillBuff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skillBuff` (
      `id` int(10) NOT NULL DEFAULT '0',
      `name` varchar(255) NOT NULL,
      `attrId` varchar(255) NOT NULL,
      `valueP` varchar(255) NOT NULL,
      `value` varchar(255) NOT NULL,
      `count` float(10,2) NOT NULL, 
      `side` tinyint(3) NOT NULL,  
      `type` tinyint(3) NOT NULL,  
      PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `skill`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skill` (
      `id` int(10) NOT NULL DEFAULT '0',
      `name` varchar(255) NOT NULL,
      `skillCondId` int(10) NOT NULL DEFAULT '0',
      `skillScopeId` int(10) NOT NULL DEFAULT '0',
      `skillEffectId` int(10) NOT NULL DEFAULT '0',
      `cd` float(10,2) NOT NULL,
      `actionBeforeCd` float(10,2) NOT NULL,
      `actionCostCd` float(10,2) NOT NULL,
      `actionBackCd` float(10,2) NOT NULL,
      `frozeTime` float(10,2) NOT NULL,
      `mpCost` int(10) NOT NULL DEFAULT '0',
      `superSkill` tinyint(3) NOT NULL,  
      `attackCount` tinyint(3) NOT NULL,  
      PRIMARY KEY (`id`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `map`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map` (
      `id` int(10) NOT NULL DEFAULT '0',
      `floor` tinyint(3) NOT NULL DEFAULT '0', 
      `info` varchar(255) NOT NULL,
      PRIMARY KEY (`id`,`floor`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `chance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chance` (
      `id` int(10) NOT NULL DEFAULT '0',
      `level` tinyint(3) NOT NULL DEFAULT '0', 
      `chanceW` int(10) NOT NULL,
      PRIMARY KEY (`id`,`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE `monster`;
CREATE TABLE `monster`(
    `id` int(10) NOT NULL DEFAULT '0',
    `groupId` tinyint(3) NOT NULL DEFAULT '0',
    `name` varchar(255) NOT NULL,
    `power` int(10) NOT NULL DEFAULT '0',
    `money` int(10) NOT NULL DEFAULT '0',
    `prob`  int(10) NOT NULL DEFAULT '0',
    `itemId` int(10) NOT NULL DEFAULT '0',
    `itemNum` tinyint(3) NOT NULL DEFAULT '0',    
    PRIMARY KEY(`id`,`groupId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;

DROP TABLE IF EXISTS `item_template2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_template2` (
      `id` int(11) NOT NULL,
      `name` varchar(255) NOT NULL,
      `subClass` tinyint(3) unsigned NOT NULL COMMENT '物品类型',
      `maxQuantity` int(6) NOT NULL DEFAULT '1' COMMENT '最大堆叠数量',
      `coin` int(10) NOT NULL COMMENT '交易价格上限',
      PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `battleAward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battleAward` (
      `mapId` tinyint(3) NOT NULL DEFAULT '1' COMMENT '地图ID',
      `exp`   int(10) NOT NULL DEFAULT '0' COMMENT '经验',
      `moneyNum`   int(10) NOT NULL DEFAULT '0' COMMENT '铜钱数量',
      `itemIds`  varchar(255) NOT NULL,
      `itemNums`  varchar(255) NOT NULL,
      PRIMARY KEY (`mapId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `corps_campaign_base`;
CREATE TABLE `corps_campaign_base` (
      `battleId` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '战役Id',
      `explimit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '军团声望限制',
      `forcenum`   int(10) unsigned NOT NULL DEFAULT '0' COMMENT '一张战役地图上的势力的个数',
      `playermin`  int(3) unsigned NOT NULL DEFAULT '0' COMMENT '玩家个数下限',
      `playermax`  int(3) unsigned NOT NULL DEFAULT '0' COMMENT '玩家的个数上限',
      PRIMARY KEY (`battleId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
