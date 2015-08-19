-- MySQL dump 10.13  Distrib 5.1.53, for pc-linux-gnu (x86_64)                                                                    
--
-- Host: localhost    Database: asss
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

SET NAMES UTF8;
DROP TABLE IF EXISTS `player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player` (
    `id` bigint(20) unsigned NOT NULL,
    `name` varchar(255) NOT NULL,

    PRIMARY KEY (`id`),
    KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `gvar`;  
CREATE TABLE `gvar` (
    `id` int(10) unsigned NOT NULL,
    `data` int(10) unsigned NOT NULL,
    `over` int(10) unsigned NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `var`; 
CREATE TABLE `var` (
    `playerId` bigint(10) unsigned NOT NULL,
    `id` smallint(5) NOT NULL,
    `data` int(10) unsigned NOT NULL,
    `over` int(10) unsigned NOT NULL,
    PRIMARY KEY (`playerId`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_id`; 
CREATE TABLE `player_id` (
    `id` bigint(20) unsigned NOT NULL,
    `phoneId` varchar(255) NOT NULL DEFAULT '',
    `accounts` varchar(255) NOT NULL DEFAULT '',   /*帐号*/
    /*PRIMARY KEY (`phoneId`,`accounts`)*/
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `fvar`; 
CREATE TABLE `fvar` (
    `playerId` bigint(10) unsigned NOT NULL,
    `fighterId` int(10) unsigned NOT NULL,
    `id` smallint(5) NOT NULL,
    `data` int(10) unsigned NOT NULL,
    `over` int(10) unsigned NOT NULL,
    PRIMARY KEY (`playerId`,`fighterId`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `fighter`; 
CREATE TABLE `fighter` (
    `playerId` bigint(10) unsigned NOT NULL,
    `fighterId` int(10) unsigned NOT NULL,
    `experience` bigint(20) unsigned NOT NULL DEFAULT '0',
    `weapon` int(10) unsigned NOT NULL DEFAULT '0',
    `armor1` int(10) unsigned NOT NULL DEFAULT '0',
    `armor2` int(10) unsigned NOT NULL DEFAULT '0',
    `armor3` int(10) unsigned NOT NULL DEFAULT '0',
    `armor4` int(10) unsigned NOT NULL DEFAULT '0',
    `armor5` int(10) unsigned NOT NULL DEFAULT '0',
    `addTime` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`fighterId`,`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `friends`; 
CREATE TABLE `friends` (
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL,
    `friendId` bigint(20) unsigned NOT NULL,
    PRIMARY KEY (`type`,`playerId`,`friendId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `item`; 
CREATE TABLE `item` (
    `playerId` bigint(20) unsigned NOT NULL,
    `itemId` int(10) unsigned NOT NULL DEFAULT '0',
    `count` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`itemId`,`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `clan`; 
CREATE TABLE `clan` (
    `clanId` int(10) unsigned NOT NULL DEFAULT '0',
    `name` varchar(255) NOT NULL DEFAULT '',   /*帐号*/
    `picIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `announcement` varchar(255) NOT NULL DEFAULT '',   /*帐号*/
    `announcement2` varchar(255) NOT NULL DEFAULT '',   /*帐号*/
    `creater` bigint(20) unsigned NOT NULL,
    `leader` bigint(20) unsigned NOT NULL,
    `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `contribute` int(10) unsigned NOT NULL DEFAULT '0',
    `personMax` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `battleRoomId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '公会战房间id',
    `clanFame` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '公会声望',
    `conquests` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '公会战绩',
    `forceId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '公会战所属势力',
    PRIMARY KEY (`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `clan_player`; 
CREATE TABLE `clan_player` (
    `clanId` int(10) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL,
    `position` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `contribute` int(10) unsigned NOT NULL DEFAULT '0',
    `enterTime` int(10) unsigned NOT NULL DEFAULT '0',
    `isClanBattle` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '是否参加公会战',
    PRIMARY KEY (`clanId`,`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `mail`; 
CREATE TABLE `mail` (
    `id` int(10) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL,
    `contextId` int(10) unsigned NOT NULL DEFAULT '0',
    `items` varchar(255) NOT NULL,
    `option` tinyint(3) NOT NULL DEFAULT '0',
    `overTime` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_apply_clan`; 
CREATE TABLE `player_apply_clan` (
    `clanId` int(10) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL,
    `time` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`ClanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `reportid`; 
CREATE TABLE `reportid` (
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `maxid` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `clan_battle_pos`; 
CREATE TABLE `clan_battle_pos` (
    `roomId`   int(10) unsigned NOT NULL DEFAULT '0' COMMENT '房间id',
    `mapId`    tinyint(3)  unsigned NOT NULL DEFAULT '0' COMMENT '地图id',
    `playerId` bigint(20) unsigned NOT NULL COMMENT '玩家Id',
    `fighterId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战将id',
    `posx`     int(3) unsigned NOT NULL DEFAULT '0' COMMENT '地图上的坐标x',
    `posy`     int(3) unsigned NOT NULL DEFAULT '0' COMMENT '地图上的坐标y',
    `mainFighterHP` int(3) unsigned NOT NULL DEFAULT '0' COMMENT '主将血量',
    `soldiersHP` varchar(1024) NOT NULL DEFAULT '' COMMENT '小兵们的血量',
    PRIMARY KEY (`mapId`,`playerId`,`fighterId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `clan_battle_room`; 
CREATE TABLE `clan_battle_room` (
    `roomId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '房间Id',
    `forceId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '势力Id',
    `battleId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '战役Id',
    `clans`    varchar(1024) NOT NULL DEFAULT '' COMMENT '军团id们',
    `fighterNum` int(3) unsigned NOT NULL DEFAULT '0' COMMENT '某一势力的战将数量',
    `buildTime`  int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建房间的时间',
    PRIMARY KEY (`roomId`,`forceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `clan_battle_comment`; 
CREATE TABLE `clan_battle_comment` (
    `roomId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '房间Id',
    `forceId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '势力Id',
    `mapId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '地图id',
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '留言者Id',
    `message` varchar(1024) NOT NULL DEFAULT '' COMMENT '留言',
    PRIMARY KEY (`roomId`,`forceId`,`mapId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `clan_battle_order`; 
CREATE TABLE `clan_battle_order` (
    `roomId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '房间Id',
    `forceId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '势力Id',
    `mapId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '地图id',
    `order` tinyint(3)  NOT NULL DEFAULT '0' COMMENT '军团令',
    PRIMARY KEY (`roomId`,`forceId`,`mapId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `report2id`; 
CREATE TABLE `report2id` (
    `roomId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '房间Id',
    `cityId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '城市Id',
    `actId`  int(3) unsigned NOT NULL DEFAULT '0' COMMENT '回合Id',
    `reportId` int(10)  NOT NULL DEFAULT '0' COMMENT '战术战报Id',
    `time`  int(10) NOT NULL DEFAULT '0' COMMENT '战术发生时间',
    PRIMARY KEY (`roomId`,`cityId`,`actId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `clan_battle_citystatus`; 
CREATE TABLE `clan_battle_citystatus` (
    `roomId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '房间Id',
    `battleId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '战役Id',
    `cityId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '城市Id',
    `ownforce`  tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '这座城被哪个势力占领了',
    PRIMARY KEY (`roomId`,`cityId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `account_pwd` (
      `accounts` varchar(255) NOT NULL DEFAULT '',
      `password` varchar(255) NOT NULL DEFAULT '',
      PRIMARY KEY (`accounts`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8

