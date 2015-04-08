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
    `itemId` int(10) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL,
    `count` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`itemId`,`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `clan`; 
CREATE TABLE `clan` (
    `clanId` int(10) unsigned NOT NULL DEFAULT '0',
    `name` varchar(255) NOT NULL DEFAULT '',   /*帐号*/
    `announcement` varchar(255) NOT NULL DEFAULT '',   /*帐号*/
    `creater` bigint(20) unsigned NOT NULL,
    `leader` bigint(20) unsigned NOT NULL,
    `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `contribute` int(10) unsigned NOT NULL DEFAULT '0',
    `personMax` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `clan_player`; 
CREATE TABLE `clan_player` (
    `clanId` int(10) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL,
    `position` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `contribute` int(10) unsigned NOT NULL DEFAULT '0',
    `enterTime` int(10) unsigned NOT NULL DEFAULT '0',
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




