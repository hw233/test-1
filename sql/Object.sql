-- MySQL dump 10.13  Distrib 5.1.53, for pc-linux-gnu (x86_64)
--
-- Host: localhost    Database: asss
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

SET NAMES UTF8;

--
-- Table structure for table `SpecialAward`
--

DROP TABLE IF EXISTS `SpecialAward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SpecialAward` (
  `rank` tinyint(2) unsigned NOT NULL,
  `type` tinyint(1) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `awardtime` int(10) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `announce`
--

DROP TABLE IF EXISTS `announce`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `announce` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `startTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '起始时间，以周期时间取整加此时间开始',
  `interval` int(10) unsigned NOT NULL DEFAULT '3600' COMMENT '间隔周期时间',
  `color` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '颜色: 0-白色 1-绿色 2-蓝色 3-紫色 4-橙色 5-暗金 6-红 7-粉',
  `endTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '结束时间，小于当前时间表示从现在开始的指定秒数内结束公告，大于当前时间表示指定时间戳结束，0表示无穷发送',
  `msg` varchar(4095) NOT NULL DEFAULT '' COMMENT '公告内容',
  `link` varchar(255) NOT NULL DEFAULT '' COMMENT '点击公告后跳转的链接',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `arena_bet`
--

DROP TABLE IF EXISTS `arena_bet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `arena_bet` (
  `id` bigint(20) unsigned NOT NULL,
  `round` tinyint(3) unsigned NOT NULL,
  `state` tinyint(3) unsigned NOT NULL,
  `group` tinyint(3) unsigned NOT NULL,
  `recieved` tinyint(3) unsigned NOT NULL,
  `pos` smallint(4) unsigned NOT NULL,
  `tael` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`,`state`,`round`,`group`,`pos`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `athletics_rank`
--

DROP TABLE IF EXISTS `athletics_rank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `athletics_rank` (
  `row` tinyint(1) NOT NULL DEFAULT '0',
  `rank` int(10) NOT NULL DEFAULT '0',
  `ranker` bigint(20) NOT NULL DEFAULT '0',
  `maxRank` smallint(4) NOT NULL DEFAULT '0',
  `challengeNum` tinyint(1) NOT NULL DEFAULT '0',
  `challengeTime` int(10) NOT NULL DEFAULT '0',
  /*`boxColor` tinyint(1) NOT NULL DEFAULT '0',*/
  /*`boxType` tinyint(1) NOT NULL DEFAULT '0',*/
  /*`boxCount` int(10) NOT NULL DEFAULT '0',*/
  /*`boxFlushTime` int(10) NOT NULL DEFAULT '0',*/
  `prestige`  int(10) unsigned NOT NULL DEFAULT '0',
  `tael`  int(10) unsigned NOT NULL DEFAULT '0',
  `winStreak` smallint(3) unsigned NOT NULL DEFAULT '0',
  `bewinstreak` smallint(3) unsigned NOT NULL DEFAULT '0',
  `failstreak` smallint(3) unsigned NOT NULL DEFAULT '0',
  `befailstreak` smallint(3) unsigned NOT NULL DEFAULT '0',
  `oldrank` int(10) NOT NULL DEFAULT '0',
  `first4rank` int(10) unsigned NOT NULL DEFAULT '0',
  `extrachallenge` int(10) unsigned NOT NULL DEFAULT '0',
  `pageNum` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `eChallengeTime` int(10) NOT NULL DEFAULT '0',
  `ePhysical` tinyint(3) NOT NULL DEFAULT '30',
  `eSelectIndex` tinyint(3) NOT NULL DEFAULT '0',
  `eCombine1` int(10) NOT NULL DEFAULT '0',
  `eCombine2` int(10) NOT NULL DEFAULT '0',
  `eCombine3` int(10) NOT NULL DEFAULT '0',
  `eCombine4` int(10) NOT NULL DEFAULT '0',
  `eCombine5` int(10) NOT NULL DEFAULT '0',
  `eRival1` bigint(20) NOT NULL DEFAULT '0',
  `eRival2` bigint(20) NOT NULL DEFAULT '0',
  `eRival3` bigint(20) NOT NULL DEFAULT '0',
  `eRival4` bigint(20) NOT NULL DEFAULT '0',
  `eRival5` bigint(20) NOT NULL DEFAULT '0',
  `eCanAttack1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eCanAttack2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eCanAttack3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eCanAttack4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eCanAttack5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eRivalType1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eRivalType2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eRivalType3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eRivalType4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `eRivalType5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ranker`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `athletics_record`
--

DROP TABLE IF EXISTS `athletics_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `athletics_record` (
  `id` int(10) NOT NULL,
  `atker` bigint(20) NOT NULL,
  `defer` bigint(20) NOT NULL,
  `repid` int(10) NOT NULL,
  `time` int(10) NOT NULL,
  `winSide` tinyint(1) NOT NULL DEFAULT '0',
  /*`awardType` tinyint(1) NOT NULL DEFAULT '0',*/
  /*`awardAtkerCount` int(10) NOT NULL DEFAULT '0',*/
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `athletics_event`
--

DROP TABLE IF EXISTS `athletics_event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `athletics_event` (
  `id` int(10) NOT NULL,
  `row` tinyint(1) NOT NULL DEFAULT '0',
  `player1` bigint(20) NOT NULL,
  `player2` bigint(20) NOT NULL,
  `cond` tinyint(3) NOT NULL DEFAULT '0',
  `color` tinyint(3) NOT NULL DEFAULT '0',
  `value` smallint(5) unsigned NOT NULL,
  `itemcount` tinyint(3) NOT NULL DEFAULT '0',
  `itemid` int(10) NOT NULL,
  `time` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `auto_battle`
--

DROP TABLE IF EXISTS `auto_battle`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auto_battle` (
  `playerId` bigint(20) unsigned NOT NULL,
  `npcId` int(10) unsigned NOT NULL,
  `count` mediumint(5) unsigned NOT NULL,
  `interval` mediumint(5) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `boss`
--

DROP TABLE IF EXISTS `boss`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `boss` (
  `id` int(10) unsigned NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `pos` tinyint(3) unsigned NOT NULL,
  `hp` int(10) unsigned NOT NULL,
  UNIQUE KEY `id_pos` (`id`,`pos`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `boss_damage`
--

DROP TABLE IF EXISTS `boss_damage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `boss_damage` (
  `id` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `damage` int(10) unsigned NOT NULL,
  `exp` int(10) unsigned NOT NULL,
  UNIQUE KEY `id_playerId` (`id`,`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_battle_result`
--

DROP TABLE IF EXISTS `clan_battle_result`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_battle_result` (
  `id` int(10) unsigned NOT NULL,
  `battleTime` int(10) unsigned NOT NULL,
  `result` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`battleTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_battler`
--

DROP TABLE IF EXISTS `clan_battler`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_battler` (
  `id` int(10) unsigned NOT NULL,
  `battler` bigint(20) unsigned NOT NULL,
  `battlerLev` int(10) unsigned NOT NULL,
  `battleClanId` int(10) unsigned NOT NULL,
  `battleClanTime` int(10) unsigned NOT NULL,
  `battleHold` int(10) unsigned NOT NULL,
  `battleStatus` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `reliveNum` smallint(5) unsigned NOT NULL,
  `wins` smallint(5) unsigned NOT NULL,
  `serialWins` smallint(5) unsigned NOT NULL,
  `maxSerialWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `southEdurance` smallint(5) unsigned NOT NULL,
  `northEdurance` smallint(5) unsigned NOT NULL,
  `grabAchieve` smallint(5) unsigned NOT NULL,
  `enterCount` tinyint(3) unsigned NOT NULL,
  `hasEnter` tinyint(3) unsigned NOT NULL,
  `hasAttack` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_donate_record`
--

DROP TABLE IF EXISTS `clan_donate_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_donate_record` (
  `clanId` int(10) unsigned NOT NULL,
  `donateName` varchar(255) NOT NULL DEFAULT '',
  `techId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `donateCount` int(10) unsigned NOT NULL DEFAULT '0',
  `donateTime` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_pending_player`
--

DROP TABLE IF EXISTS `clan_pending_player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_pending_player` (
  `id` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `class` tinyint(3) unsigned NOT NULL,
  `opTime` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`,`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_pending_reward`
--

DROP TABLE IF EXISTS `clan_pending_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_pending_reward` (
  `id` int(10) unsigned NOT NULL,
  `timeAlloc` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `itemId` int(10) unsigned NOT NULL,
  `itemNum` int(10) unsigned NOT NULL,
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_pet_friendliness`
--

DROP TABLE IF EXISTS `clan_pet_friendliness`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_pet_friendliness` (
  `memberId` bigint(20) unsigned NOT NULL,
  `clanId` int(10) unsigned NOT NULL,
  `petId` smallint(5) unsigned NOT NULL,
  `friendliness` int(10) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_player`
--

DROP TABLE IF EXISTS `clan_player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_player` (
  `id` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `joinTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成员 - 加入时间    待审核 - 申请时间',
  `proffer` int(10) unsigned NOT NULL,
  `cls` int(10) unsigned NOT NULL COMMENT '帮派职位',
  `enterCount` tinyint(3) unsigned NOT NULL,
  `thisDay` int(10) unsigned NOT NULL DEFAULT '0',
  `petFriendness1` smallint(5) unsigned NOT NULL,
  `petFriendness2` smallint(5) unsigned NOT NULL,
  `petFriendness3` smallint(5) unsigned NOT NULL,
  `petFriendness4` smallint(5) unsigned NOT NULL,
  `favorCount1` smallint(5) unsigned NOT NULL,
  `lastFavorTime1` smallint(5) unsigned NOT NULL,
  `favorCount2` smallint(5) unsigned NOT NULL,
  `lastFavorTime2` smallint(5) unsigned NOT NULL,
  `favorCount3` smallint(5) unsigned NOT NULL,
  `lastFavorTime3` smallint(5) unsigned NOT NULL,
  `favorCount4` smallint(5) unsigned NOT NULL,
  `lastFavorTime4` smallint(5) unsigned NOT NULL,
  `signupRankBattleTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '报名帮会排名战时间',
  `rankBattleField` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '帮会排名战战役',  
  PRIMARY KEY (`playerId`),
  KEY `id` (`id`),
  KEY `playerId` (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_repo`
--

DROP TABLE IF EXISTS `clan_repo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_repo` (
  `id` int(10) unsigned NOT NULL,
  `itemId` int(10) unsigned NOT NULL,
  `itemNum` int(10) unsigned NOT NULL,
  UNIQUE KEY `id_itemId` (`id`,`itemId`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_reward_record`
--

DROP TABLE IF EXISTS `clan_reward_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_reward_record` (
  `id` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `timeAlloc` int(10) unsigned NOT NULL,
  `playerName` varchar(255) NOT NULL,
  `items` varchar(4095) NOT NULL DEFAULT '',
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_tech`
--

DROP TABLE IF EXISTS `clan_tech`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_tech` (
  `clanId` int(10) unsigned NOT NULL,
  `techId` smallint(5) unsigned NOT NULL,
  `level` smallint(5) unsigned NOT NULL,
  `extra` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`clanId`,`techId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clan_skill`
--

DROP TABLE IF EXISTS `clan_skill`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_skill` (
  `playerId` bigint(20) unsigned NOT NULL,
  `skillId` smallint(5) unsigned NOT NULL,
  `level` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`playerId`, `skillId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clanbattle_clan_reward`
--

DROP TABLE IF EXISTS `clanbattle_clan_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clanbattle_clan_reward` (
  `id` int(10) unsigned NOT NULL,
  `itemNum` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clanbattle_player_reward`
--

DROP TABLE IF EXISTS `clanbattle_player_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clanbattle_player_reward` (
  `id` bigint(20) unsigned NOT NULL,
  `itemNum` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `data_latest`
--

DROP TABLE IF EXISTS `data_latest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `data_latest` (
  `announce` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_task_instance`
--

DROP TABLE IF EXISTS `day_task_instance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_task_instance` (
  `loopTask` int(10) unsigned NOT NULL,
  `count` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '???ɴ???',
  `maxCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `maxFlushQualityCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `preTaskId` int(10) unsigned NOT NULL DEFAULT '0',
  `preFlushTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '?ϴ?ˢ??ʱ??',
  `preTaskQuality` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '?ϴ?ˢ????????ɫ??\\r\\n1 ??ɫ\\r\\n2 ??ɫ\\r\\n3 ��ɫ\\r\\n4 ??ɫ\\r\\n5 ??ɫ',
  `autoCompletedTaskId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '?Զ????????????????ţ???????0?? ??ʾû??',
  `autoCompletedTaskAcceptTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '?Զ?????????????ʱ??',
  `ownerId` bigint(20) unsigned NOT NULL,
  `currDay` int(10) unsigned NOT NULL COMMENT '????ʱ??',
  `flushCount` int(10) unsigned NOT NULL COMMENT '当前颜色刷新次数',
  PRIMARY KEY (`loopTask`,`ownerId`),
  KEY `ownerId` (`ownerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dungeon_auto`
--

DROP TABLE IF EXISTS `dungeon_auto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dungeon_auto` (
  `playerId` bigint(20) unsigned NOT NULL,
  `dungeonId` int(10) unsigned NOT NULL,
  `totalExp` int(10) unsigned NOT NULL,
  `won` tinyint(1) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dungeon_player`
--

DROP TABLE IF EXISTS `dungeon_player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dungeon_player` (
  `id` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `count` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `totalCount` int(10) unsigned NOT NULL DEFAULT '0',
  `firstPass` int(10) unsigned NOT NULL DEFAULT '0',
  `counterEnd` int(10) unsigned NOT NULL DEFAULT '0',
  `justice` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `justice_roar` tinyint(3) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `id_playerId` (`id`,`playerId`),
  KEY `id` (`id`),
  KEY `playerId` (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `equipment`
--

DROP TABLE IF EXISTS `equipment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `equipment` (
  `id` int(10) unsigned NOT NULL,
  `itemId` int(10) unsigned NOT NULL,
  `enchant` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tRank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `maxTRank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `trumpExp` int(10) unsigned NOT NULL DEFAULT '0',
  `attrType1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attrValue1` smallint(6) NOT NULL DEFAULT '0',
  `attrType2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attrValue2` smallint(6) NOT NULL DEFAULT '0',
  `attrType3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attrValue3` smallint(6) NOT NULL DEFAULT '0',
  `sockets` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `socket1` int(10) unsigned NOT NULL DEFAULT '0',
  `socket2` int(10) unsigned NOT NULL DEFAULT '0',
  `socket3` int(10) unsigned NOT NULL DEFAULT '0',
  `socket4` int(10) unsigned NOT NULL DEFAULT '0',
  `socket5` int(10) unsigned NOT NULL DEFAULT '0',
  `socket6` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `equipment_spirit`
--

DROP TABLE IF EXISTS `equipment_spirit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `equipment_spirit` (
  `id` int(10) unsigned NOT NULL,
  `splev1` smallint(6) NOT NULL DEFAULT '0',
  `splev2` smallint(6) NOT NULL DEFAULT '0',
  `splev3` smallint(6) NOT NULL DEFAULT '0',
  `splev4` smallint(6) NOT NULL DEFAULT '0',
  `spform1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `spform2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `spform3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `exchange_ticket`
--

DROP TABLE IF EXISTS `exchange_ticket`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `exchange_ticket` (
  `playerId` bigint(20) unsigned NOT NULL,
  `exchange_num` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `fighter`
--

DROP TABLE IF EXISTS `fighter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fighter` (
  `id` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `potential` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  `capacity` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `relvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `experience` bigint(20) unsigned NOT NULL DEFAULT '0',
  `practiceExp` bigint(20) unsigned NOT NULL DEFAULT '0',
  `hp` int(10) unsigned NOT NULL DEFAULT '0',
  `attrType1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attrValue1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `attrType2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attrValue2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `attrType3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attrValue3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `fashion` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon` int(10) unsigned NOT NULL DEFAULT '0',
  `armor1` int(10) unsigned NOT NULL DEFAULT '0',
  `armor2` int(10) unsigned NOT NULL DEFAULT '0',
  `armor3` int(10) unsigned NOT NULL DEFAULT '0',
  `armor4` int(10) unsigned NOT NULL DEFAULT '0',
  `armor5` int(10) unsigned NOT NULL DEFAULT '0',
  `ring` int(10) unsigned NOT NULL DEFAULT '0',
  `amulet` int(10) unsigned NOT NULL DEFAULT '0',
  `peerless` mediumint(5) unsigned NOT NULL DEFAULT '0',
  `talent` mediumint(5) unsigned NOT NULL DEFAULT '0',
  `trump` varchar(255) NOT NULL,
  `acupoints` varchar(255) NOT NULL,
  `skill` varchar(255) NOT NULL,
  `citta` varchar(255) NOT NULL,
  `skills` varchar(255) NOT NULL,
  `cittas` varchar(255) NOT NULL,
  `summoned` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `hideFashion` tinyint(4) DEFAULT '0',
  UNIQUE KEY `id_playerId` (`id`,`playerId`),
  KEY `playerId` (`playerId`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `fighter_buff`
--

DROP TABLE IF EXISTS `fighter_buff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fighter_buff` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` int(10) unsigned NOT NULL,
  `buffId` mediumint(5) unsigned NOT NULL,
  `data` int(10) unsigned NOT NULL,
  UNIQUE KEY `playerId_id` (`playerId`,`id`),
  KEY `id` (`id`),
  KEY `playerId` (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `fighter_train`
--

DROP TABLE IF EXISTS `fighter_train`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fighter_train` (
  `fgtId` int(11) NOT NULL,
  `ownerId` bigint(20) unsigned NOT NULL,
  `priceType` tinyint(4) NOT NULL,
  `price` smallint(6) NOT NULL,
  `trainTime` int(11) NOT NULL,
  `trainRemain` int(11) NOT NULL,
  `checkTime` int(11) NOT NULL,
  `accExp` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`fgtId`,`ownerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `practice_place`;
CREATE TABLE `practice_place` (
  `id` tinyint(3) NOT NULL,
  `ownerid` bigint(20) unsigned NOT NULL,
  `protid` bigint(20) unsigned NOT NULL,
  `maxslot` smallint(5) unsigned NOT NULL DEFAULT '30',
  `openslot` smallint(5) unsigned NOT NULL DEFAULT '0',
  `protmoney` smallint(5) unsigned NOT NULL DEFAULT '10',
  `slotmoney` smallint(5) unsigned NOT NULL DEFAULT '10',
  `open` tinyint(2) unsigned NOT NULL DEFAULT '1',
  `enemyCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `winCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `slotincoming` int(11) NOT NULL,
  `protincoming` int(11) NOT NULL,
   PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `practice_data`;
CREATE TABLE `practice_data` (
  `id` bigint(20) unsigned NOT NULL,
  `place` tinyint(3) NOT NULL,
  `slot` smallint(5) unsigned NOT NULL DEFAULT '30',
  `type` tinyint(3) NOT NULL,
  `pricetype` tinyint(3) NOT NULL,
  `slotprice` smallint(5) NOT NULL,
  `protprice` smallint(5) NOT NULL,
  `traintime` int(11) NOT NULL,
  `checktime` int(11) NOT NULL,
  `prot` tinyint(3) NOT NULL,
  `cdend` int(11) NOT NULL,
  `winnerid` bigint(20) unsigned NOT NULL,
  `fighters` varchar(255) NOT NULL,
   PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `friend`
--

DROP TABLE IF EXISTS `friend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friend` (
  `id` bigint(20) unsigned NOT NULL,
  `type` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `friendId` bigint(20) unsigned NOT NULL,
  UNIQUE KEY `id_friendId` (`id`,`friendId`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `friendliness`
--

DROP TABLE IF EXISTS `friendliness`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friendliness` (
  `playerId` bigint(20) unsigned NOT NULL,
  `fighterId` int(10) unsigned NOT NULL,
  `friendliness` smallint(5) unsigned NOT NULL,
  `favorSubmitCount` int(5) NOT NULL DEFAULT '0',
  `favorSubmitDay` int(11) NOT NULL DEFAULT '0',
  UNIQUE KEY `playerId_fighterId` (`playerId`,`fighterId`),
  KEY `playerId` (`playerId`),
  KEY `fighterId` (`fighterId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `item`
--

DROP TABLE IF EXISTS `item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item` (
  `id` int(10) unsigned NOT NULL COMMENT '如果是装备类物品， 则是流水号；否则是物品编号',
  `itemNum` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '物品叠加数量',
  `ownerId` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '物品所属',
  `bindType` smallint(5) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `id_ownerId_bindType` (`id`,`ownerId`,`bindType`),
  KEY `id` (`id`),
  KEY `ownerId` (`ownerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `locked_player`
--

DROP TABLE IF EXISTS `locked_player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locked_player` (
  `player_id` bigint(20) unsigned NOT NULL,
  `lockExpireTime` int(10) unsigned NOT NULL,
  PRIMARY KEY (`player_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mail`
--

DROP TABLE IF EXISTS `mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mail` (
  `mailId` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `sender` varchar(255) NOT NULL,
  `recvTime` int(10) unsigned NOT NULL,
  `flag` tinyint(3) unsigned NOT NULL,
  `title` varchar(255) NOT NULL,
  `content` varchar(8192) NOT NULL,
  `additionalId` int(10) unsigned NOT NULL,
  KEY `mailId` (`mailId`),
  KEY `playerId` (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mail_package`
--

DROP TABLE IF EXISTS `mail_package`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mail_package` (
  `id` int(10) unsigned NOT NULL,
  `itemId` int(10) unsigned NOT NULL,
  `itemCount` int(10) unsigned NOT NULL,
  UNIQUE KEY `id_itemId` (`id`,`itemId`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pass_word`
--

DROP TABLE IF EXISTS `pass_word`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pass_word` (
  `playerId` bigint(20) unsigned NOT NULL,
  `password` varchar(255) NOT NULL,
  `questionForPwd` varchar(255) NOT NULL,
  `answerForPwd` varchar(255) NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `tripod`;
CREATE TABLE `tripod` (
  `id` bigint(20) unsigned NOT NULL,
  `soul` int(10) unsigned NOT NULL DEFAULT '0',
  `fire` tinyint(1) unsigned NOT NULL,
  `quality` tinyint(1) unsigned NOT NULL,
  `awdst` tinyint(1) unsigned NOT NULL,
  `regen` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `itemId` int(10) unsigned NOT NULL,
  `num` tinyint(5) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `player`
--

DROP TABLE IF EXISTS `player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player` (
  `id` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  `coupon` int(10) unsigned NOT NULL DEFAULT '0',
  `tael` int(10) unsigned NOT NULL DEFAULT '0',
  `coin` int(10) unsigned NOT NULL DEFAULT '100000',
  `prestige`  int(10) unsigned NOT NULL DEFAULT '0',
  `status` int(10) unsigned NOT NULL DEFAULT '0',
  `country` tinyint(1) unsigned NOT NULL,
  `title` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `titleAll` varchar(255) NOT NULL DEFAULT '0|',
  `archievement` int(10) unsigned NOT NULL DEFAULT '0',
  `attainment` int(10) unsigned NOT NULL DEFAULT '0',
  `qqvipl` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `qqvipyear` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `qqawardgot` int(11) unsigned NOT NULL DEFAULT '0',
  `qqawardEnd` int(10) unsigned NOT NULL DEFAULT '0',
  `ydGemId` int(10) unsigned NOT NULL DEFAULT '0',
  `location` smallint(5) unsigned NOT NULL,
  `inCity` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `lastOnline` int(10) unsigned NOT NULL DEFAULT '0',
  `packSize` smallint(4) unsigned NOT NULL DEFAULT '100',
  `mounts` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mainFighter` int(10) unsigned NOT NULL DEFAULT '0',
  `icCount` varchar(32) NOT NULL DEFAULT '',
  `piccount` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `nextpicreset` int(10) unsigned NOT NULL DEFAULT '0',
  `formation` smallint(5) unsigned NOT NULL DEFAULT '0',
  `lineup` varchar(255) NOT NULL DEFAULT '',
  `bossLevel` smallint(4) unsigned NOT NULL DEFAULT '21',
  `totalRecharge` int(10) unsigned NOT NULL DEFAULT '0',
  `lastExp` int(10) unsigned NOT NULL DEFAULT '0',
  `lastResource` bigint(20) unsigned NOT NULL DEFAULT '0',
  `nextReward` varchar(32) NOT NULL DEFAULT '',
  `nextExtraReward` int(10) unsigned NOT NULL DEFAULT '0',
  `tavernId` varchar(255) NOT NULL DEFAULT '',
  `bookStore` varchar(255) NOT NULL DEFAULT '0',
  `shimen` varchar(255) NOT NULL DEFAULT '',
  `fshimen` varchar(255) NOT NULL DEFAULT '',
  `yamen` varchar(255) NOT NULL DEFAULT '',
  `fyamen` varchar(255) NOT NULL DEFAULT '',
  `clantask` varchar(255) NOT NULL DEFAULT '',
  `copyFreeCnt` tinyint(3) unsigned NOT NULL DEFAULT '0', 
  `copyGoldCnt` tinyint(3) unsigned NOT NULL DEFAULT '0', 
  `copyupdate` int(10) unsigned NOT NULL DEFAULT '0',
  `frontFreeCnt` tinyint(3) unsigned NOT NULL DEFAULT '0', 
  `frontGoldCnt` tinyint(3) unsigned NOT NULL DEFAULT '0', 
  `frontUpdate` int(10) unsigned NOT NULL DEFAULT '0',
  `formations` varchar(255) NOT NULL DEFAULT '',
  `atohicfg` varchar(255) NOT NULL DEFAULT '',
  `gmLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `wallow` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `dungeonCnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `dungeonEnd` int(10) unsigned NOT NULL DEFAULT '0',
  `newGuild` bigint(20) unsigned NOT NULL DEFAULT '0',
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `name` (`name`),
  KEY `mainFighter` (`mainFighter`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_buff`
--

DROP TABLE IF EXISTS `player_buff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_buff` (
  `id` bigint(20) NOT NULL,
  `buffId` smallint(5) NOT NULL,
  `data` int(10) unsigned NOT NULL,
  UNIQUE KEY `id_buffId` (`id`,`buffId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `prepaid`
--

DROP TABLE IF EXISTS `prepaid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prepaid` (
  `id` bigint(20) unsigned NOT NULL,
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sale`
--

DROP TABLE IF EXISTS `sale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sale` (
  `saleId` int(10) NOT NULL,
  `ownerId` bigint(20) NOT NULL,
  `status` smallint(6) NOT NULL,
  `saleTime` int(10) NOT NULL,
  `priceType` int(10) NOT NULL DEFAULT '0',
  `price` int(10) NOT NULL DEFAULT '0',
  `itemId` int(10) NOT NULL,
  `itemNum` int(10) NOT NULL,
  PRIMARY KEY (`saleId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `task_instance`
--

DROP TABLE IF EXISTS `task_instance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task_instance` (
  `taskId` int(10) unsigned NOT NULL,
  `ownerId` bigint(20) unsigned NOT NULL,
  `acceptTime` int(10) unsigned NOT NULL,
  `step` varchar(255) NOT NULL DEFAULT '',
  `timeBegin` int(10) unsigned NOT NULL,
  `timeEnd` int(11) NOT NULL,
  `completed` tinyint(1) unsigned NOT NULL,
  `submit` tinyint(1) unsigned NOT NULL,
  PRIMARY KEY (`taskId`,`ownerId`),
  KEY `ownerId` (`ownerId`),
  KEY `submit` (`submit`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `trade`
--

DROP TABLE IF EXISTS `trade`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trade` (
  `tradeId` int(10) unsigned NOT NULL,
  `ownerId` bigint(20) unsigned NOT NULL,
  `tradeSide` tinyint(1) unsigned NOT NULL COMMENT '0:交易方 1:被交易方',
  `tradeStatus` tinyint(1) unsigned NOT NULL,
  `tradeTo` bigint(20) unsigned NOT NULL,
  `tradeTitle` varchar(255) NOT NULL DEFAULT '',
  `tradeTime` int(10) unsigned NOT NULL,
  `coin` int(10) unsigned NOT NULL DEFAULT '0',
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  `items` varchar(255) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_id_name_mapping`
--

DROP TABLE IF EXISTS `user_id_name_mapping`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_id_name_mapping` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed
-- MySQL dump 10.13  Distrib 5.1.53, for pc-linux-gnu (x86_64)
--
-- Host: localhost    Database: asss
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
-- Table structure for table `clan`
--

DROP TABLE IF EXISTS `clan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `rank` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `funds` int(10) unsigned NOT NULL DEFAULT '0',
  `foundTime` int(10) unsigned NOT NULL DEFAULT '0',
  `founder` bigint(20) unsigned NOT NULL,
  `leader` bigint(20) unsigned NOT NULL DEFAULT '0',
  `watchman` bigint(20) unsigned NOT NULL DEFAULT '0',
  `construction` bigint(20) unsigned NOT NULL DEFAULT '0',
  `contact` varchar(64) NOT NULL DEFAULT '',
  `announce` varchar(512) NOT NULL DEFAULT '',
  `purpose` varchar(512) NOT NULL DEFAULT '',
  `proffer` int(10) unsigned NOT NULL DEFAULT '0',
  `grabAchieve` smallint(5) unsigned NOT NULL DEFAULT '0',
  `battleTime` tinyint(3) unsigned NOT NULL DEFAULT '16',
  `nextBattleTime` tinyint(3) unsigned NOT NULL DEFAULT '16',
  `favorId1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `favorId2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `favorId3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `favorId4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `favorTime` int(10) unsigned NOT NULL DEFAULT '0',
  `allyClan` int(10) unsigned NOT NULL DEFAULT '0',
  `enemyClan1` int(10) unsigned NOT NULL DEFAULT '0',
  `enemyClan2` int(10) unsigned NOT NULL DEFAULT '0',
  `battleThisDay` int(10) unsigned NOT NULL DEFAULT '0',
  `battleStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `southEdurance` smallint(5) unsigned NOT NULL DEFAULT '30',
  `northEdurance` smallint(5) unsigned NOT NULL DEFAULT '30',
  `hallEdurance` smallint(5) unsigned NOT NULL DEFAULT '30',
  `hasBattle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `battleScore` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '帮会排名战分数',
  `dailyBattleScore` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '帮会战日积分',
  `battleRanking` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上周帮会战排名', 
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clan`
--
-- WHERE:  id=0

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed
-- MySQL dump 10.13  Distrib 5.1.53, for pc-linux-gnu (x86_64)
--
-- Host: localhost    Database: asss
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
-- Table structure for table `database_version`
--

DROP TABLE IF EXISTS `database_version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `database_version` (
  `version` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `database_version`
--

LOCK TABLES `database_version` WRITE;
/*!40000 ALTER TABLE `database_version` DISABLE KEYS */;
INSERT INTO `database_version` VALUES ('Object_20110330_1');
/*!40000 ALTER TABLE `database_version` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

DROP TABLE IF EXISTS `player_copy`;
CREATE TABLE `player_copy` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` tinyint(3) unsigned NOT NULL,
  `floor` tinyint(3) unsigned NOT NULL,
  `spot` tinyint(3) unsigned NOT NULL,
  `lootlvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `player_id` (`playerId`,`id`),
  KEY `playerId` (`playerId`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_frontmap`;
CREATE TABLE `player_frontmap` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` tinyint(3) unsigned NOT NULL,
  `spot` tinyint(3) unsigned NOT NULL,
  `count` tinyint(3) unsigned NOT NULL,
  `status` tinyint(3) unsigned NOT NULL,
  `lootlvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `player_id_spot` (`playerId`,`id`, `spot`),
  KEY `playerIdId` (`playerId`, `id`),
  KEY `playerId` (`playerId`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `attainment`
--

DROP TABLE IF EXISTS `attainment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `attainment` (
  `ownerId` bigint(20) unsigned NOT NULL,
  `attainId` smallint(5) unsigned NOT NULL,
  `status` int(10) unsigned NOT NULL,
  `updatetime` int(10) unsigned NOT NULL,
   PRIMARY KEY (`ownerId`,`attainId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `activityData`;
CREATE TABLE `activityData` (
   `playerId` bigint(20) unsigned NOT NULL,
   `overTime` int(10) unsigned NOT NULL,
   `awardId`  int(10) unsigned NOT NULL,
    `point`   int(10) unsigned NOT NULL,
    `award`   int(10) unsigned NOT NULL,
   `flags`    varchar(255) NOT NULL DEFAULT '',
    `scores`  int(10) unsigned NOT NULL,
    `propsID` int(10) unsigned NOT NULL,
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `worldboss`;
CREATE TABLE `worldboss` (
  `npcId` int(10) unsigned NOT NULL,
  `level` tinyint(3) unsigned NOT NULL,
  `location` smallint(5) unsigned NOT NULL,
  `count` tinyint(5) unsigned NOT NULL,
  PRIMARY KEY (`npcId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `autocopy`;
CREATE TABLE `autocopy` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `auto_frontmap`;
CREATE TABLE `auto_frontmap` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `recharge`;
CREATE TABLE `recharge` (
  `no` char(255) NOT NULL DEFAULT '',
  `playerId` bigint(20) unsigned NOT NULL,
  `id` smallint(4) NOT NULL DEFAULT '29999',
  `num` int(10) unsigned NOT NULL,
  `status` tinyint(3) unsigned NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`no`),
  KEY noplayer (`no`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `var`;
CREATE TABLE `var` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` smallint(5) NOT NULL,
  `data` int(10) unsigned NOT NULL,
  `over` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `teamcopy_player`;
CREATE TABLE `teamcopy_player` (
  `playerId` bigint(20) unsigned NOT NULL,
  `copyId` tinyint(3) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `pass` tinyint(3) unsigned NOT NULL,
  `passTimes` tinyint(3) unsigned NOT NULL,
  `vTime` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`,`copyId`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `teamcopy_player_award`;
CREATE TABLE `teamcopy_player_award` (
  `playerId` bigint(20) unsigned NOT NULL,
  `rollId` tinyint(3) unsigned NOT NULL,
  `roll` tinyint(3) unsigned NOT NULL,
  `awardId` int(10) unsigned NOT NULL,
  `awardCnt` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `clan_item`;
CREATE TABLE `clan_item` (
  `clanid` int(10) unsigned NOT NULL,
  `playerid` bigint(20) unsigned NOT NULL,
  `itemid` int(10) unsigned NOT NULL,
  `itemnum` int(10) unsigned DEFAULT 0,
  PRIMARY KEY(`clanid`,`playerid`, `itemid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `clan_item_history`;
CREATE TABLE `clan_item_history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `clanid` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `time` int(10) unsigned NOT NULL,
  `playerid` bigint(20) unsigned NOT NULL,
  `itemstr` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `player_honeyfall`;
CREATE TABLE `player_honeyfall` (
  `playerId` bigint(20) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `value` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`, `type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `money`;
CREATE TABLE `money` (
  `time` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `gold` int(10) unsigned NOT NULL,
  `coupon` int(10) unsigned NOT NULL,
  `tael` int(10) unsigned NOT NULL,
  `achievement` int(10) unsigned NOT NULL,
  `prestige` int(10) unsigned NOT NULL,
  PRIMARY KEY (`time`, `type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `heromemo`;
CREATE TABLE `heromemo` (
  `playerId` bigint(20) unsigned NOT NULL,
  `awards` varchar(255) NOT NULL DEFAULT '',
  `memos` varchar(512) NOT NULL DEFAULT '',
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `towndeamon_player`;
CREATE TABLE `towndeamon_player` (
  `deamonLevel` smallint(3) unsigned NOT NULL,
  `curLevel` smallint(3) unsigned NOT NULL,
  `maxLevel` smallint(3) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `startTime` int(10) unsigned NOT NULL,
  `accTime` int(10) unsigned NOT NULL,
  `accLen` int(10) unsigned NOT NULL,
  `accAwards` int(10) unsigned NOT NULL,
  `vitalityTime` int(10) unsigned NOT NULL,
  `vitality` int(10) unsigned NOT NULL,
  `spirit` int(10) unsigned NOT NULL,
  `challengeTime` int(10) unsigned NOT NULL,
  `itemId` int(10) unsigned NOT NULL DEFAULT 0,
  `itemNum` int(10) unsigned NOT NULL DEFAULT 0,
  `quitLevel` smallint(3) unsigned NOT NULL DEFAULT 0,
  `attacker` bigint(20) unsigned NOT NULL DEFAULT 0,
  `time2MaxLvl` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `cfriend_awards`;
CREATE TABLE `cfriend_awards` (
  `playerId` bigint(20) unsigned NOT NULL,
  `invitedId` bigint(20) unsigned NOT NULL DEFAULT 0,
  `awards` varchar(256) NOT NULL DEFAULT '',
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `second_soul`;
CREATE TABLE `second_soul` (
  `fighterId` int(10) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `cls` tinyint(3) unsigned NOT NULL,
  `xinxiu` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `practiceLevel` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `stateLevel` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `stateExp` int(10) unsigned NOT NULL DEFAULT 0,
  `skills` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY(`fighterId`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `enchlog`;
CREATE TABLE `enchlog` (
  `playerId` bigint(20) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `enchant` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`playerId`,`type`,`enchant`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `wboss`;
CREATE TABLE `wboss` (
  `idx` tinyint(3) unsigned NOT NULL,
  `last` int(10) unsigned NOT NULL,
  `hp` int(10) unsigned NOT NULL,
  `atk` int(10) NOT NULL,
  `matk` int(10) NOT NULL,
  PRIMARY KEY (`idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `discount`;
CREATE TABLE `discount` (
      `itemid` int(10) unsigned NOT NULL,
      `discount` tinyint(3) unsigned NOT NULL,
      `timeBegin` int(10) unsigned NOT NULL,
      `timeEnd` int(10) unsigned NOT NULL,
      `priceOriginal` int(10) unsigned NOT NULL,
      `priceDiscount` int(10) unsigned NOT NULL,
      `type` tinyint(3) unsigned NOT NULL,
      `count` int(10) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `shuoshuo`;
CREATE TABLE `shuoshuo` (
  `playerId` bigint(20) unsigned NOT NULL,
  `updateTime` int(10) NOT NULL,
  `shuoshuo` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `elixir`;
CREATE TABLE `elixir` (
    `id` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `strength` int(5) NOT NULL DEFAULT '0',
    `physique` int(5) NOT NULL DEFAULT '0',
    `agility` int(5) NOT NULL DEFAULT '0',
    `intelligence` int(5) NOT NULL DEFAULT '0',
    `will` int(5) NOT NULL DEFAULT '0',
    `soul` int(5) NOT NULL DEFAULT '0',
    `attack` int(5) NOT NULL DEFAULT '0',
    `defend` int(5) NOT NULL DEFAULT '0',
    `critical` int(5) NOT NULL DEFAULT '0',
    `pierce` int(5) NOT NULL DEFAULT '0',
    `evade` int(5) NOT NULL DEFAULT '0',
    `counter` int(5) NOT NULL DEFAULT '0',
    `tough` int(5) NOT NULL DEFAULT '0',
    `action` int(5) NOT NULL DEFAULT '0',
    UNIQUE KEY `id_playerId` (`id`,`playerId`),
    KEY `playerId` (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `luckylog`;
CREATE TABLE `luckylog` (
    `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
    `name` varchar(255) NOT NULL,
    `items` varchar(512) NOT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `real_item_award` (
    `id` int(10) unsigned NOT NULL,
    `cd` int(10) unsigned NOT NULL DEFAULT '0',
    `card_no` varchar(255) NOT NULL,
    `card_psw` varchar(255) NOT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `rechargenextret` (
    `id` bigint(20) unsigned NOT NULL,
    `record` varchar(1024) NOT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `gvar`;
CREATE TABLE `gvar` (
  `id` int(10) unsigned NOT NULL,
  `data` int(10) unsigned NOT NULL,
  `over` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `new_relation`;
CREATE TABLE IF NOT EXISTS `new_relation` (
    `playerId` bigint(20) unsigned NOT NULL,
    `mood` smallint(3) unsigned NOT NULL DEFAULT 0,
    `sign` varchar(255) NOT NULL,
    PRIMARY KEY(`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `skill_strengthen`;
CREATE TABLE IF NOT EXISTS `skill_strengthen` (
    `id` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `skillid` smallint(5) unsigned NOT NULL,
    `father` smallint(5) unsigned NOT NULL,
    `maxVal` int(10) unsigned NOT NULL,
    `curVal` int(10) unsigned NOT NULL,
    `lvl` tinyint(1) unsigned NOT NULL,
    `maxLvl` tinyint(1) unsigned NOT NULL,
    UNIQUE KEY `id_playerId_skill` (`id`,`playerId`, `skillid`),
    KEY `playerId` (`playerId`),
    KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `tianjie`;
CREATE TABLE IF NOT EXISTS `tianjie` (                                                                             
    `id` int(10) unsigned NOT NULL AUTO_INCREMENT,                                                       
    `is_opened` tinyint(1) DEFAULT '0' COMMENT '是否已开启',                                             
    `is_execute` tinyint(1) DEFAULT '0' COMMENT '是否正在进行天劫事件',                                  
    `is_finish` tinyint(1) DEFAULT '0' COMMENT '是否天劫事件已达成',                                     
    `is_ok` tinyint(1) DEFAULT '0' COMMENT '是否BOSS已击杀 ',                                            
    `level` int(11) DEFAULT '0' COMMENT '天劫等级',                                                      
    `rate` int(11) DEFAULT '0' COMMENT '天劫进度 1,2,3,4',                                               
    `opentime` datetime DEFAULT NULL COMMENT '天劫事件开启时间',                                         
    `r1_killed` char(32) DEFAULT NULL COMMENT '事件1怪物击杀数',                                         
    `r2_donated` char(32) DEFAULT NULL COMMENT '事件2捐款数',                                            
    `r3_copyid` int(11) DEFAULT '0' COMMENT '事件3天雷阵层数',                                           
    `r4_day` int(11) DEFAULT '0' COMMENT '打BOSS的天数',                                                 
    `open_next` tinyint(4) DEFAULT '0',
    `is_wait` tinyint(4) DEFAULT '0',
    `is_manual` tinyint(4) DEFAULT '0' COMMENT '是否手动开启的天劫', 
    `is_touch` tinyint(4) DEFAULT '0' COMMENT '是否被触发',
    PRIMARY KEY (`id`),                                                                                  
    UNIQUE KEY `level_only` (`level`)                                                                    
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

DROP TABLE IF EXISTS `qixi`;
CREATE TABLE IF NOT EXISTS `qixi` (
    `playerId` bigint(20) unsigned NOT NULL,
    `lover` bigint(20) unsigned NOT NULL,
    `bind` tinyint(3) unsigned NOT NULL,
    `pos` tinyint(3) unsigned NOT NULL,
    `event` tinyint(3) unsigned NOT NULL,
    `score` int(10) unsigned NOT NULL,
    PRIMARY KEY(`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `clan_statue` (
    `clanId` bigint(20) unsigned NOT NULL,
    `level` int(10) unsigned NOT NULL,
    `exp` bigint(20) unsigned NOT NULL,
    `expUpdateTime` bigint(20) unsigned NOT NULL,
    PRIMARY KEY(`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `clan_copy` (
    `clanId` bigint(20) unsigned NOT NULL,
    `level` int(10) unsigned NOT NULL,
    `levelUpdateTime` bigint(20) unsigned NOT NULL,
    `maxCopyLevel` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '帮派通过最高副本等级',
    `maxCopyTime`  bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '帮派通过最高副本等级时间戳',
    PRIMARY KEY(`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `clan_copy_log` (
    `clanId` bigint(20) unsigned NOT NULL,
    `logTime` bigint(20) unsigned NOT NULL,
    `logType` smallint(6) NOT NULL,
    `playerName` varchar(255) NOT NULL,
    `logVal` bigint(20) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `reportid`;
CREATE TABLE `reportid` (                
    `maxid` int(11) NOT NULL DEFAULT '0',  
    PRIMARY KEY (`maxid`)                  
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

