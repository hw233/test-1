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

create database data;
use data;

--
-- Table structure for table `area`
--

DROP TABLE IF EXISTS `area`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `area` (
  `id` int(10) unsigned NOT NULL,
  `side` int(1) unsigned NOT NULL DEFAULT '1',
  `area` varchar(255) NOT NULL DEFAULT '0',
  `rate` varchar(255) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area`
--
-- ORDER BY:  `id`

LOCK TABLES `area` WRITE;
/*!40000 ALTER TABLE `area` DISABLE KEYS */;
INSERT INTO `area` VALUES (1,1,'0,0','1.05'),(2,1,'0,0;1,0;2,0;3,0;4,0;-1,0;-2,0;-3,0;-4,0','0.8,*0.5,*0.5,*0.5,*0.5,/0.5,*0.5,*0.5,*0.5'),(3,1,'0,0;1,0;-1,0;0,1;0,-1','1,0.2,0.2,0.2,0.2'),(4,1,'0,0;0,1;0,2;0,3;0,4','0.8,*0.5,*0.5,*0.5,*0.5'),(5,1,'0,0;0,1','0.95,0.25'),(6,1,'0,0;-1,1;1,1','0.9,0.3,0.3'),(7,1,'0,0','1.05'),(10,1,'0,0;1,0;2,0;3,0;4,0;-1,0;-2,0;-3,0;-4,0;0,1;1,1;2,1;3,1;4,1;-1,1;-2,1;-3,1;-4,1;0,2;1,2;2,2;3,2;4,2;-1,2;-2,2;-3,2;-4,2;0,3;1,3;2,3;3,3;4,3;-1,3;-2,3;-3,3;-4,3;0,4;1,4;2,4;3,4;4,4;-1,4;-2,4;-3,4;-4,4','1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1');
/*!40000 ALTER TABLE `area` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `attr_extra`
--

DROP TABLE IF EXISTS `attr_extra`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `attr_extra` (
  `id` int(10) NOT NULL,
  `strength` varchar(10) NOT NULL DEFAULT '0',
  `physique` varchar(10) NOT NULL DEFAULT '0',
  `agility` varchar(10) NOT NULL DEFAULT '0',
  `intelligence` varchar(10) NOT NULL DEFAULT '0',
  `will` varchar(10) NOT NULL DEFAULT '0',
  `soul` varchar(10) NOT NULL DEFAULT '0',
  `aura` varchar(10) NOT NULL DEFAULT '0',
  `auraMax` varchar(10) NOT NULL DEFAULT '0',
  `tough` varchar(10) NOT NULL DEFAULT '0',
  `attack` varchar(64) NOT NULL DEFAULT '0',
  `mag_attack` varchar(64) NOT NULL DEFAULT '0',
  `defend` varchar(64) NOT NULL DEFAULT '0',
  `mag_defend` varchar(64) NOT NULL DEFAULT '0',
  `hp` varchar(64) NOT NULL DEFAULT '0',
  `skills` varchar(255) NOT NULL DEFAULT '',
  `action` float(10,4) NOT NULL DEFAULT '0.0000',
  `hitrate` float(10,4) NOT NULL DEFAULT '0.0000',
  `evade` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical_dmg` float(10,4) NOT NULL DEFAULT '0.0000',
  `pierce` float(10,4) NOT NULL DEFAULT '0.0000',
  `counter` float(10,4) NOT NULL DEFAULT '0.0000',
  `mag_res` float(10,4) NOT NULL DEFAULT '0.0000',
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
-- Dumping data for table `boss`
--
-- ORDER BY:  `id`

LOCK TABLES `boss` WRITE;
/*!40000 ALTER TABLE `boss` DISABLE KEYS */;
INSERT INTO `boss` VALUES (4367,13316,43200,7200,30),(4368,13573,59400,10800,45),(4369,16388,75600,21600,55);
/*!40000 ALTER TABLE `boss` ENABLE KEYS */;
UNLOCK TABLES;

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

--
-- Table structure for table `clan_skill_template`
--

DROP TABLE IF EXISTS `clan_skill_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_skill_template` (
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
-- Dumping data for table `clan_skill_template`
--
-- ORDER BY:  `id`,`level`

LOCK TABLES `clan_skill_template` WRITE;
/*!40000 ALTER TABLE `clan_skill_template` DISABLE KEYS */;
INSERT INTO `clan_skill_template` VALUES (1,0,0,0,0,0),(1,1,0,0,0,0),(1,2,200,1,40,0),(1,3,500,2,100,0),(1,4,800,3,160,0),(1,5,1200,4,240,0),(1,6,2000,5,400,0),(1,7,3000,6,600,0),(1,8,4000,7,800,0),(1,9,5000,8,1000,0),(1,10,6000,9,1200,0),(1,11,7500,10,1500,0),(1,12,9000,11,1800,0),(1,13,10500,12,2100,0),(1,14,12000,13,2400,0),(1,15,13500,14,2700,0),(1,16,15000,15,3000,0),(1,17,17000,16,3400,0),(1,18,19000,17,3800,0),(1,19,21000,18,4200,0),(1,20,23000,19,4600,0),(1,21,25000,20,5000,0),(1,22,27000,21,5400,0),(1,23,29000,22,5800,0),(1,24,31000,23,6200,0),(1,25,33000,24,6600,0),(1,26,35000,25,7000,0),(1,27,37000,26,7400,0),(1,28,39000,27,7800,0),(1,29,41000,28,8200,0),(1,30,43000,29,8600,0),(1,31,44000,30,8800,0),(1,32,45000,31,9000,0),(1,33,46000,32,9200,0),(1,34,47000,33,9400,0),(1,35,48000,34,9600,0),(1,36,49000,35,9800,0),(1,37,50000,36,10000,0),(1,38,51000,37,10200,0),(1,39,52000,38,10400,0),(1,40,53000,39,10600,0),(1,41,54000,40,10800,0),(1,42,55000,41,11000,0),(1,43,56000,42,11200,0),(1,44,57000,43,11400,0),(1,45,58000,44,11600,0),(1,46,59000,45,11800,0),(1,47,60000,46,12000,0),(1,48,61000,47,12200,0),(1,49,62000,48,12400,0),(1,50,63000,49,12600,0),(2,0,0,0,0,0),(2,1,500,1,1,0),(2,2,3000,6,2,0),(2,3,5500,11,3,0),(2,4,8000,16,4,0),(2,5,10500,21,5,0),(2,6,13000,26,6,0),(2,7,16000,31,7,0),(2,8,21000,36,8,0),(2,9,26000,41,9,0),(2,10,31000,46,10,0),(3,0,0,0,0,0),(3,1,200,1,31,0),(3,2,400,2,32,0),(3,3,600,3,33,0),(3,4,800,4,34,0),(3,5,1000,5,35,0),(3,6,1200,6,36,0),(3,7,1400,7,37,0),(3,8,1600,8,38,0),(3,9,1800,9,39,0),(3,10,2000,10,40,0),(3,11,2200,11,41,0),(3,12,2400,12,42,0),(3,13,2600,13,43,0),(3,14,2800,14,44,0),(3,15,3000,15,45,0),(3,16,3200,16,46,0),(3,17,3400,17,47,0),(3,18,3600,18,48,0),(3,19,3800,19,49,0),(3,20,4000,20,50,0),(3,21,4200,21,51,0),(3,22,4400,22,52,0),(3,23,4600,23,53,0),(3,24,4800,24,54,0),(3,25,5000,25,55,0),(3,26,5200,26,56,0),(3,27,5400,27,57,0),(3,28,5600,28,58,0),(3,29,5800,29,59,0),(3,30,6000,30,60,0),(3,31,6400,31,61,0),(3,32,6800,32,62,0),(3,33,7200,33,63,0),(3,34,7600,34,64,0),(3,35,8000,35,65,0),(3,36,8400,36,66,0),(3,37,8800,37,67,0),(3,38,9200,38,68,0),(3,39,9600,39,69,0),(3,40,10000,40,70,0),(3,41,10400,41,71,0),(3,42,10800,42,72,0),(3,43,11200,43,73,0),(3,44,11600,44,74,0),(3,45,12000,45,75,0),(3,46,12400,46,76,0),(3,47,12800,47,77,0),(3,48,13200,48,78,0),(3,49,13600,49,79,0),(3,50,14000,50,80,0),(4,0,0,0,0,0),(4,1,200,1,31,0),(4,2,400,2,32,0),(4,3,600,3,33,0),(4,4,800,4,34,0),(4,5,1000,5,35,0),(4,6,1200,6,36,0),(4,7,1400,7,37,0),(4,8,1600,8,38,0),(4,9,1800,9,39,0),(4,10,2000,10,40,0),(4,11,2200,11,41,0),(4,12,2400,12,42,0),(4,13,2600,13,43,0),(4,14,2800,14,44,0),(4,15,3000,15,45,0),(4,16,3200,16,46,0),(4,17,3400,17,47,0),(4,18,3600,18,48,0),(4,19,3800,19,49,0),(4,20,4000,20,50,0),(4,21,4200,21,51,0),(4,22,4400,22,52,0),(4,23,4600,23,53,0),(4,24,4800,24,54,0),(4,25,5000,25,55,0),(4,26,5200,26,56,0),(4,27,5400,27,57,0),(4,28,5600,28,58,0),(4,29,5800,29,59,0),(4,30,6000,30,60,0),(4,31,6400,31,61,0),(4,32,6800,32,62,0),(4,33,7200,33,63,0),(4,34,7600,34,64,0),(4,35,8000,35,65,0),(4,36,8400,36,66,0),(4,37,8800,37,67,0),(4,38,9200,38,68,0),(4,39,9600,39,69,0),(4,40,10000,40,70,0),(4,41,10400,41,71,0),(4,42,10800,42,72,0),(4,43,11200,43,73,0),(4,44,11600,44,74,0),(4,45,12000,45,75,0),(4,46,12400,46,76,0),(4,47,12800,47,77,0),(4,48,13200,48,78,0),(4,49,13600,49,79,0),(4,50,14000,50,80,0),(5,0,0,0,0,0),(5,1,200,1,4478,0),(5,2,400,2,4479,0),(5,3,600,3,4480,0),(5,4,800,4,4481,0),(5,5,1000,5,4482,0),(5,6,1200,6,4483,0),(5,7,1400,7,4484,0),(5,8,1600,8,4485,0),(5,9,1800,9,4486,0),(5,10,2000,10,4487,0),(5,11,2200,11,4488,0),(5,12,2400,12,4489,0),(5,13,2600,13,4490,0),(5,14,2800,14,4491,0),(5,15,3000,15,4492,0),(5,16,3200,16,4493,0),(5,17,3400,17,4494,0),(5,18,3600,18,4495,0),(5,19,3800,19,4496,0),(5,20,4000,20,4497,0),(5,21,4200,21,4498,0),(5,22,4400,22,4499,0),(5,23,4600,23,4500,0),(5,24,4800,24,4501,0),(5,25,5000,25,4502,0),(5,26,5200,26,4503,0),(5,27,5400,27,4504,0),(5,28,5600,28,4505,0),(5,29,5800,29,4506,0),(5,30,6000,30,4507,0),(5,31,6400,31,4508,0),(5,32,6800,32,4509,0),(5,33,7200,33,4510,0),(5,34,7600,34,4511,0),(5,35,8000,35,4512,0),(5,36,8400,36,4513,0),(5,37,8800,37,4514,0),(5,38,9200,38,4515,0),(5,39,9600,39,4516,0),(5,40,10000,40,4517,0),(5,41,10400,41,4518,0),(5,42,10800,42,4519,0),(5,43,11200,43,4520,0),(5,44,11600,44,4521,0),(5,45,12000,45,4522,0),(5,46,12400,46,4523,0),(5,47,12800,47,4524,0),(5,48,13200,48,4525,0),(5,49,13600,49,4526,0),(5,50,14000,50,4527,0),(6,0,0,0,0,0),(6,1,1500,3,1,0),(6,2,4000,8,2,0),(6,3,6500,13,3,0),(6,4,9000,18,4,0),(6,5,11500,23,5,0),(6,6,14000,28,6,0),(6,7,18000,33,7,0),(6,8,23000,38,8,0),(6,9,28000,43,9,0),(6,10,33000,48,10,0),(7,0,0,0,0,0),(7,1,2500,5,1,1),(7,2,5000,10,2,2),(7,3,7500,15,2,3),(7,4,10000,20,3,4),(7,5,12500,25,3,5),(7,6,15000,30,3,6),(7,7,20000,35,3,7),(7,8,25000,40,3,8),(7,9,30000,45,4,9),(7,10,35000,50,4,10),(8,0,0,0,4540,0),(8,1,400,2,4541,0),(8,2,1400,7,4542,0),(8,3,2400,12,4543,0),(8,4,3400,17,4544,0),(8,5,4400,22,4545,0),(8,6,5400,27,4546,0),(8,7,6800,32,4547,0),(8,8,8800,37,4548,0),(8,9,10800,42,4549,0),(8,10,12800,47,4550,0),(9,0,0,0,4562,0),(9,1,800,4,4563,0),(9,2,1800,9,4564,0),(9,3,2800,14,4565,0),(9,4,3800,19,4566,0),(9,5,4800,24,4567,0),(9,6,5800,29,4568,0),(9,7,7600,34,4569,0),(9,8,9600,39,4570,0),(9,9,11600,44,4571,0),(9,10,13600,49,4572,0);
/*!40000 ALTER TABLE `clan_skill_template` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `dungeon`
--
-- ORDER BY:  `id`

LOCK TABLES `dungeon` WRITE;
/*!40000 ALTER TABLE `dungeon` DISABLE KEYS */;
INSERT INTO `dungeon` VALUES (1,'焚骨窟',8710,0,30),(2,'天人福邸',13061,0,50),(3,'困仙阵',16645,0,70),(4,'无间炼狱',25607,0,80),(5,'海底幻境',25093,0,90);
/*!40000 ALTER TABLE `dungeon` ENABLE KEYS */;
UNLOCK TABLES;

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

--
-- Dumping data for table `dungeon_level`
--
-- ORDER BY:  `id`,`level`

LOCK TABLES `dungeon_level` WRITE;
/*!40000 ALTER TABLE `dungeon_level` DISABLE KEYS */;
INSERT INTO `dungeon_level` VALUES (11,1,101,'1045,1046'),(11,2,102,'1045,1046'),(11,3,103,'1045,1046'),(11,4,104,'1045,1046'),(11,5,105,'1047,1048,1049,1050'),(11,6,106,'1051,1052'),(11,7,106,'1051,1052'),(11,8,108,'1051,1052'),(11,9,108,'1051,1052'),(11,10,110,'1053,1054,1055,1056'),(11,11,111,'1057,1058'),(11,12,111,'1057,1058'),(11,13,113,'1057,1058'),(11,14,113,'1057,1058'),(11,15,115,'1059,1060,1061,1062'),(12,1,101,'1063,1064'),(12,2,102,'1063,1064'),(12,3,103,'1063,1064'),(12,4,104,'1063,1064'),(12,5,105,'1065,1066,1067,1068'),(12,6,106,'1069,1070'),(12,7,106,'1069,1070'),(12,8,108,'1069,1070'),(12,9,108,'1069,1070'),(12,10,110,'1071,1072,1073,1074'),(12,11,111,'1075,1076'),(12,12,111,'1075,1076'),(12,13,113,'1075,1076'),(12,14,113,'1075,1076'),(12,15,115,'1077,1078,1079,1080'),(21,1,201,'1081,1082'),(21,2,201,'1081,1082'),(21,3,203,'1081,1082'),(21,4,203,'1081,1082'),(21,5,205,'1085,1086,1087,1088'),(21,6,206,'1093,1094'),(21,7,206,'1093,1094'),(21,8,206,'1093,1094'),(21,9,209,'1093,1094'),(21,10,209,'1093,1094'),(21,11,209,'1093,1094'),(21,12,212,'1097,1098,1099,1100'),(21,13,213,'1105,1106'),(21,14,213,'1105,1106'),(21,15,215,'1105,1106'),(21,16,215,'1105,1106'),(21,17,217,'1109,1110,1111,1112'),(21,18,218,'1117,1118'),(21,19,218,'1117,1118'),(21,20,220,'1117,1118'),(21,21,220,'1117,1118'),(21,22,222,'1121,1122,1123,1124'),(22,1,201,'1083,1084'),(22,2,201,'1083,1084'),(22,3,203,'1083,1084'),(22,4,203,'1083,1084'),(22,5,205,'1089,1090,1091,1092'),(22,6,206,'1095,1096'),(22,7,206,'1095,1096'),(22,8,206,'1095,1096'),(22,9,209,'1095,1096'),(22,10,209,'1095,1096'),(22,11,209,'1095,1096'),(22,12,212,'1101,1102,1103,1104'),(22,13,213,'1107,1108'),(22,14,213,'1107,1108'),(22,15,215,'1107,1108'),(22,16,215,'1107,1108'),(22,17,217,'1113,1114,1115,1116'),(22,18,218,'1119,1120'),(22,19,218,'1119,1120'),(22,20,220,'1119,1120'),(22,21,220,'1119,1120'),(22,22,222,'1125,1126,1127,1128'),(31,1,301,'1129,1130'),(31,2,301,'1129,1130'),(31,3,303,'1129,1130'),(31,4,303,'1129,1130'),(31,5,305,'1133,1134,1135,1136'),(31,6,306,'1141,1142'),(31,7,306,'1141,1142'),(31,8,306,'1141,1142'),(31,9,309,'1141,1142'),(31,10,309,'1141,1142'),(31,11,309,'1141,1142'),(31,12,312,'1145,1146,1147,1148'),(31,13,313,'1153,1154'),(31,14,313,'1153,1154'),(31,15,315,'1153,1154'),(31,16,316,'1153,1154'),(31,17,316,'1153,1154'),(31,18,316,'1153,1154'),(31,19,319,'1157,1158,1159,1160'),(31,20,320,'1165,1166'),(31,21,320,'1165,1166'),(31,22,320,'1165,1166'),(31,23,323,'1165,1166'),(31,24,324,'1165,1166'),(31,25,324,'1165,1166'),(31,26,326,'1169,1170,1171,1172'),(31,27,327,'1177,1178'),(31,28,327,'1177,1178'),(31,29,329,'1177,1178'),(31,30,330,'1177,1178'),(31,31,330,'1177,1178'),(31,32,332,'1181,1182,1183,1184'),(31,33,333,'1189,1190'),(31,34,333,'1189,1190'),(31,35,335,'1189,1190'),(31,36,336,'1189,1190'),(31,37,336,'1189,1190'),(31,38,338,'1193,1194,1195,1196'),(31,39,339,'1201,1202'),(31,40,339,'1201,1202'),(31,41,341,'1201,1202'),(31,42,342,'1201,1202'),(31,43,343,'1205,1206,1207,1208'),(31,44,344,'1213,1214,1215,1216'),(32,1,301,'1131,1132'),(32,2,301,'1131,1132'),(32,3,303,'1131,1132'),(32,4,303,'1131,1132'),(32,5,305,'1137,1138,1139,1140'),(32,6,306,'1143,1144'),(32,7,306,'1143,1144'),(32,8,306,'1143,1144'),(32,9,309,'1143,1144'),(32,10,309,'1143,1144'),(32,11,309,'1143,1144'),(32,12,312,'1149,1150,1151,1152'),(32,13,313,'1155,1156'),(32,14,313,'1155,1156'),(32,15,315,'1155,1156'),(32,16,316,'1155,1156'),(32,17,316,'1155,1156'),(32,18,316,'1155,1156'),(32,19,319,'1161,1162,1163,1164'),(32,20,320,'1167,1168'),(32,21,320,'1167,1168'),(32,22,320,'1167,1168'),(32,23,323,'1167,1168'),(32,24,324,'1167,1168'),(32,25,324,'1167,1168'),(32,26,326,'1173,1174,1175,1176'),(32,27,327,'1179,1180'),(32,28,327,'1179,1180'),(32,29,329,'1179,1180'),(32,30,330,'1179,1180'),(32,31,330,'1179,1180'),(32,32,332,'1185,1186,1187,1188'),(32,33,333,'1191,1192'),(32,34,333,'1191,1192'),(32,35,335,'1191,1192'),(32,36,336,'1191,1192'),(32,37,336,'1191,1192'),(32,38,338,'1197,1198,1199,1200'),(32,39,339,'1203,1204'),(32,40,339,'1203,1204'),(32,41,341,'1203,1204'),(32,42,342,'1203,1204'),(32,43,343,'1209,1210,1211,1212'),(32,44,344,'1217,1218,1219,1220'),(41,1,401,'1221,1222'),(41,2,401,'1221,1222'),(41,3,403,'1221,1222'),(41,4,403,'1221,1222'),(41,5,405,'1225,1226,1227,1228'),(41,6,406,'1233,1234'),(41,7,406,'1233,1234'),(41,8,408,'1233,1234'),(41,9,408,'1233,1234'),(41,10,410,'1237,1238,1239,1240'),(41,11,411,'1245,1246'),(41,12,411,'1245,1246'),(41,13,413,'1245,1246'),(41,14,413,'1245,1246'),(41,15,415,'1249,1250,1251,1252'),(41,16,416,'1257,1258'),(41,17,416,'1257,1258'),(41,18,418,'1257,1258'),(41,19,418,'1257,1258'),(41,20,420,'1261,1262,1263,1264'),(42,1,401,'1223,1224'),(42,2,401,'1223,1224'),(42,3,403,'1223,1224'),(42,4,403,'1223,1224'),(42,5,405,'1229,1230,1231,1232'),(42,6,406,'1235,1236'),(42,7,406,'1235,1236'),(42,8,408,'1235,1236'),(42,9,408,'1235,1236'),(42,10,410,'1241,1242,1243,1244'),(42,11,411,'1247,1248'),(42,12,411,'1247,1248'),(42,13,413,'1247,1248'),(42,14,413,'1247,1248'),(42,15,415,'1253,1254,1255,1256'),(42,16,416,'1259,1260'),(42,17,416,'1259,1260'),(42,18,418,'1259,1260'),(42,19,418,'1259,1260'),(42,20,420,'1265,1266,1267,1268'),(51,1,501,'1269,1270'),(51,2,501,'1269,1270'),(51,3,503,'1269,1270'),(51,4,503,'1269,1270'),(51,5,505,'1273,1274,1275,1276'),(51,6,506,'1281,1282'),(51,7,506,'1281,1282'),(51,8,508,'1281,1282'),(51,9,508,'1281,1282'),(51,10,510,'1285,1286,1287,1288'),(51,11,511,'1293,1294'),(51,12,511,'1293,1294'),(51,13,513,'1293,1294'),(51,14,513,'1293,1294'),(51,15,515,'1297,1298,1299,1230'),(51,16,516,'1305,1306'),(51,17,516,'1305,1306'),(51,18,518,'1305,1306'),(51,19,518,'1305,1306'),(51,20,520,'1309,1310,1311,1312'),(52,1,501,'1271,1272'),(52,2,501,'1271,1272'),(52,3,503,'1271,1272'),(52,4,503,'1271,1272'),(52,5,505,'1277,1278,1279,1280'),(52,6,506,'1283,1284'),(52,7,506,'1283,1284'),(52,8,508,'1283,1284'),(52,9,508,'1283,1284'),(52,10,510,'1289,1290,1291,1292'),(52,11,511,'1295,1296'),(52,12,511,'1295,1296'),(52,13,513,'1295,1296'),(52,14,513,'1295,1296'),(52,15,515,'1301,1302,1303,1304'),(52,16,516,'1307,1308'),(52,17,516,'1307,1308'),(52,18,518,'1307,1308'),(52,19,518,'1307,1308'),(52,20,520,'1313,1314,1315,1316');
/*!40000 ALTER TABLE `dungeon_level` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dungeon_monster`
--

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

--
-- Dumping data for table `dungeon_monster`
--
-- ORDER BY:  `id`

LOCK TABLES `dungeon_monster` WRITE;
/*!40000 ALTER TABLE `dungeon_monster` DISABLE KEYS */;
INSERT INTO `dungeon_monster` VALUES (101,0,'4373,4377|4,5',3000),(102,0,'4374,4378|4,5',3600),(103,0,'4375,4379|5,6',4200),(104,0,'4376,4380|5,6',4800),(105,1,'4381,17|4376,1,3,5,9|4380,11,13',6000),(106,0,'4382,4383|6,7',5400),(108,0,'4384,4385|7,8',5400),(110,1,'4386,17|4382,2,5,9|4384,11,13,21,23',6600),(111,0,'4387,4388|7,8',6000),(113,0,'4389,4390|7,8',6600),(115,1,'4391,22|4388,0,1,3,4,7|4390,11,13,15,19',7500),(201,0,'4392,4393|5,6',7500),(203,0,'4392,4393|6,7',7500),(205,1,'4394,17|4393,4,5,8,11|4392,15,19,22',7800),(206,0,'4395,4396|6,7',8400),(209,0,'4397,4398|7,8',8400),(212,1,'4399,12|4400,0,4,16,18|4398,6,8,20,24',9000),(213,0,'4401,4402|7,8',9000),(215,0,'4401,4402|8,9',9000),(217,1,'4403,22|4404,21|4405,23|4406,0,2,4,6,7,8,10,14',11400),(218,0,'4407,4409|8,9',11400),(220,0,'4408,4409|9,10',11400),(222,1,'4410,2|4411,12|4412,16|4413,18|4408,0,4,20,24',14400),(301,0,'4419,4420|5,6',2400),(303,0,'4421,4422|6,7',3200),(305,1,'4423,17|4421,5,6,8,9|4420,16,18,22',4000),(306,0,'4424,4425|6,7',4000),(309,0,'4426,4427|7,8',4200),(312,1,'4428,17|4426,5,6,8,9,12|4424,21,22,23',4400),(313,0,'4429,4430|7,8',4400),(315,0,'4429,4430|8,9',4400),(316,0,'4443,4444|8,9',4600),(319,1,'4445,22|4446,23|4447,21|4443,1,3,5,6,8,9,12,17',5000),(320,0,'4448,4449|9,10',4800),(323,1,'4450,23|4451,21|4448,5,6,18,19|4449,8,9,15,16',5000),(324,0,'4433,4434|9,10',5200),(326,1,'4452,18|4453,16|4433,1,3,10,12,14|4434,5,7,9,20,24',5400),(327,0,'4433,4434|9,10',5200),(329,1,'4431,18|4432,16|4433,1,3,10,12,14|4434,5,7,9,20,24',5600),(330,0,'4433,4434|9,10',5200),(332,1,'4435,18|4436,16|4433,1,3,10,12,14|4434,5,7,9,20,24',5800),(333,0,'4437,4438|11,12',5200),(335,1,'4439,23|4440,21|4437,6,7,8,15,19|4438,0,2,4,16,18',6500),(336,0,'4437,4438|11,12',5200),(338,1,'4441,23|4442,21|4437,6,7,8,15,19|4438,0,2,4,16,18',6500),(339,0,'4437,4438|11,12',5200),(341,1,'4454,18|4455,16|4456,12|4437,0,1,3,4,5,6,7,8,9,21,22,23',6500),(342,0,'4437,4438|11,12',5200),(343,1,'4457,12|4458,16|4459,18|4437,0,1,3,4,5,6,7,8,9,21,22,23',6500),(344,1,'4460,12|4437,1,3,5,6,7,8,9|4438,15,16,18,19,21,22,23',6500),(401,0,'4697,4698|5,6',8000),(403,0,'4697,4698|6,7',8000),(405,1,'4699,17|4697,16,18,22|4698,5,6,8,9',8500),(406,0,'4700,4701|6,7',8500),(408,0,'4700,4701|7,8',8500),(410,1,'4702,17|4700,5,9,12,21,22,23|4701,11,13',9000),(411,0,'4703,4704|8,9',9000),(413,0,'4703,4704|9,10',9000),(415,1,'4705,17|4706,19|4703,0,2,4,15,23|4704,6,8,21',9500),(416,0,'4707,4708|10,11',9500),(418,0,'4707,4708|11,12',9500),(420,1,'4709,19|4710,15|4707,0,2,4,11,13,21,22,23|4708,5,6,8,9',10000),(501,0,'4723,4724|6,7',12000),(503,0,'4723,4724|7,8',12000),(505,1,'4725,17|4723,6,9,14,18|4724,5,8,10,16',13000),(506,0,'4726,4727|7,8',13000),(508,0,'4726,4727|8,9',13000),(510,1,'4730,23|4729,17|4728,21|4726,0,2,4,11,13,15,19',15000),(511,0,'4731,4732|8,9',14000),(513,0,'4731,4732|9,10',14000),(515,1,'4738,22|4739,11|4740,8|4731,3,4,5,6,13,14,15,18',19000),(516,0,'4736,4737|10,11',17000),(518,0,'4736,4737|11,12',17000),(520,1,'4733,22|4734,16|4735,18|4737,0,1,3,4,5,6,9,10,13,14,20,24',20000);
/*!40000 ALTER TABLE `dungeon_monster` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `equipment_set`
--

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
-- Dumping data for table `equipment_set`
--
-- ORDER BY:  `itemId`

LOCK TABLES `equipment_set` WRITE;
/*!40000 ALTER TABLE `equipment_set` DISABLE KEYS */;
INSERT INTO `equipment_set` VALUES (261,'力·定军套装',30001,30002,30003,30004),(281,'敏·定军套装',30005,30006,30007,30008),(301,'体·定军套装',30009,30010,30011,30012),(321,'智·定军套装',30013,30014,30015,30016),(341,'火·白泽套装',30017,30018,30019,30020),(361,'风·白泽套装',30021,30022,30023,30024),(381,'金·白泽套装',30025,30026,30027,30028),(401,'水·白泽套装',30029,30030,30031,30032),(461,'力·逍遥套装',30033,30034,30035,30036),(481,'敏·逍遥套装',30037,30038,30039,30040),(501,'体·逍遥套装',30041,30042,30043,30044),(521,'智·逍遥套装',30045,30046,30047,30048),(541,'火·饕餮套装',30049,30050,30051,30052),(561,'风·饕餮套装',30053,30054,30055,30056),(581,'金·饕餮套装',30057,30058,30059,30060),(601,'水·饕餮套装',30061,30062,30063,30064),(661,'力·龙纹套装',30065,30066,30067,30068),(681,'敏·龙纹套装',30069,30070,30071,30072),(701,'体·龙纹套装',30073,30074,30075,30076),(721,'智·龙纹套装',30077,30078,30079,30080),(741,'火·麒麟套装',30081,30082,30083,30084),(761,'风·麒麟套装',30085,30086,30087,30088),(781,'金·麒麟套装',30089,30090,30091,30092),(801,'水·麒麟套装',30093,30094,30095,30096),(861,'力·天助套装',30097,30098,30099,30100),(881,'敏·天助套装',30101,30102,30103,30104),(901,'体·天助套装',30105,30106,30107,30108),(921,'智·天助套装',30109,30110,30111,30112),(941,'火·凤凰套装',30113,30114,30115,30116),(961,'风·凤凰套装',30117,30118,30119,30120),(981,'金·凤凰套装',30121,30122,30123,30124),(1001,'水·凤凰套装',30125,30126,30127,30128),(1061,'力·失却套装',30129,30130,30131,30132),(1081,'敏·失却套装',30133,30134,30135,30136),(1101,'体·失却套装',30137,30138,30139,30140),(1121,'智·失却套装',30141,30142,30143,30144),(1141,'火·混沌套装',30145,30146,30147,30148),(1161,'风·混沌套装',30149,30150,30151,30152),(1181,'金·混沌套装',30153,30154,30155,30156),(1201,'水·混沌套装',30157,30158,30159,30160),(1221,'和戎套装',30041,30042,30043,30044),(1241,'殄虏套装',30073,30074,30075,30076),(1261,'虎牙套装',30105,30106,30107,30108),(1281,'骠骑套装',30137,30138,30139,30140),(1401,'焚骨套装',30161,30162,30163,30164),(1501,'天人套装',30165,30166,30167,30168),(1521,'锐·震天套装',30169,30170,30171,30172),(1541,'固·震天套装',30173,30174,30175,30176),(1561,'锐·盘龙套装',30177,30178,30179,30180),(1581,'固·盘龙套装',30181,30182,30183,30184),(1601,'锐·轩辕套装',30185,30186,30187,30188),(1621,'固·轩辕套装',30189,30190,30191,30192),(1641,'困仙套装',30193,30194,30195,30196),(1661,'炼狱套装',30197,30198,30199,30200),(1681,'锐·乾坤套装',30201,30202,30203,30204),(1701,'固·乾坤套装',30205,30206,30207,30208),(1721,'锐·女娲套装',30209,30210,30211,30212),(1741,'固·女娲套装',30213,30214,30215,30216),(1761,'幻境套装',30217,30218,30219,30220);
/*!40000 ALTER TABLE `equipment_set` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `formation`
--

DROP TABLE IF EXISTS `formation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `formation` (
  `id` tinyint(3) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `eyes` varchar(255) NOT NULL DEFAULT '',
  `grids` varchar(255) NOT NULL DEFAULT '',
  `leastNum` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `skillId` mediumint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `formation`
--
-- ORDER BY:  `id`

LOCK TABLES `formation` WRITE;
/*!40000 ALTER TABLE `formation` DISABLE KEYS */;
INSERT INTO `formation` VALUES (1,'犄角阵','25','6,0,0,0,0,10001|8,0,0,0,0,10002|12,0,0,0,0,10003',2,0),(11,'四象归本阵','25','6,0,0,0,0,10011|8,0,0,0,0,10012|16,0,0,0,0,10013|18,0,0,0,0,10014',3,0),(21,'Lv.1奇煌阵','12','6,0,0,12,2,10019|8,0,0,17,2,10020|12,0,0,0,0,10021|17,0,0,0,0,10022|22,17,2,0,0,10023',3,0),(22,'Lv.2奇煌阵','12','6,0,0,12,3,10024|8,0,0,17,3,10025|12,0,0,0,0,10026|17,0,0,0,0,10027|22,17,3,0,0,10028',3,0),(23,'Lv.3奇煌阵','12','6,0,0,12,4,10029|8,0,0,17,4,10030|12,0,0,0,0,10031|17,0,0,0,0,10032|22,17,4,0,0,10033',3,0),(24,'Lv.4奇煌阵','12','6,0,0,12,5,10034|8,0,0,17,5,10035|12,0,0,0,0,10036|17,0,0,0,0,10037|22,17,5,0,0,10038',3,0),(31,'Lv.1禁锢之阵','12','6,16,2,12,2,10039|8,18,2,0,0,10040|12,0,0,0,0,10041|16,0,0,0,0,10042|18,0,0,0,0,10043',3,0),(32,'Lv.2禁锢之阵','12','6,16,3,12,3,10044|8,18,3,0,0,10045|12,0,0,0,0,10046|16,0,0,0,0,10047|18,0,0,0,0,10048',3,0),(33,'Lv.3禁锢之阵','12','6,16,4,12,4,10049|8,18,4,0,0,10050|12,0,0,0,0,10051|16,0,0,0,0,10052|18,0,0,0,0,10053',3,0),(34,'Lv.4禁锢之阵','12','6,16,5,12,5,10054|8,18,5,0,0,10055|12,0,0,0,0,10056|16,0,0,0,0,10057|18,0,0,0,0,10058',3,0),(41,'Lv.1玄翼之阵','13','7,0,0,11,6,10059|11,15,2,0,0,10060|13,19,2,0,0,10061|15,0,0,0,0,10062|19,0,0,0,0,10063',3,0),(42,'Lv.2玄翼之阵','13','7,0,0,11,8,10064|11,15,3,0,0,10065|13,19,3,0,0,10066|15,0,0,0,0,10067|19,0,0,0,0,10068',3,0),(43,'Lv.3玄翼之阵','13','7,0,0,11,10,10069|11,15,4,0,0,10070|13,19,4,0,0,10071|15,0,0,0,0,10072|19,0,0,0,0,10073',3,0),(44,'Lv.4玄翼之阵','13','7,0,0,11,12,10074|11,15,5,0,0,10075|13,19,5,0,0,10076|15,0,0,0,0,10077|19,0,0,0,0,10078',3,0),(51,'Lv.1九死连环','10','6,0,0,10,2,10079|8,14,2,0,0,10080|10,0,0,0,0,10081|12,0,0,0,0,10082|14,0,0,0,0,10083',3,0),(52,'Lv.2九死连环','10','6,0,0,10,3,10084|8,14,3,0,0,10085|10,0,0,0,0,10086|12,0,0,0,0,10087|14,0,0,0,0,10088',3,0),(53,'Lv.3九死连环','10','6,0,0,10,4,10089|8,14,4,0,0,10090|10,0,0,0,0,10091|12,0,0,0,0,10092|14,0,0,0,0,10093',3,0),(54,'Lv.4九死连环','10','6,0,0,10,5,10094|8,14,5,0,0,10095|10,0,0,0,0,10096|12,0,0,0,0,10097|14,0,0,0,0,10098',3,0);
/*!40000 ALTER TABLE `formation` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_template`
--

DROP TABLE IF EXISTS `item_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_template` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `subClass` tinyint(3) NOT NULL COMMENT '物品类型',
  `reqLev` smallint(6) NOT NULL DEFAULT '1' COMMENT '等级需求',
  `coin` int(10) NOT NULL COMMENT '铜币售价',
  `quality` tinyint(3) NOT NULL COMMENT '品质',
  `maxQuantity` smallint(6) NOT NULL DEFAULT '1' COMMENT '最大堆叠数量',
  `bindType` tinyint(1) NOT NULL COMMENT '绑定类型',
  `data` smallint(6) NOT NULL COMMENT '武器: 武器定义; 可使用道具: 作用数值',
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
  `formationId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `experience` int(10) unsigned NOT NULL DEFAULT '0',
  `lootId` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `skills`
--

DROP TABLE IF EXISTS `skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skills` (
  `id` smallint(5) NOT NULL,
  `name` varchar(255) NOT NULL,
  `target` tinyint(3) NOT NULL DEFAULT '0',
  `cond` smallint(5) NOT NULL DEFAULT '0',
  `prob` float(10,4) NOT NULL DEFAULT '0.0000',
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
  `stateprob` float(10,4) NOT NULL DEFAULT '0.0000',
  `immune` tinyint(3) NOT NULL DEFAULT '0',
  `damage` varchar(255) NOT NULL DEFAULT '',
  `magdam` varchar(255) NOT NULL DEFAULT '',
  `hp` varchar(255) NOT NULL DEFAULT '',
  `aura` varchar(255) NOT NULL DEFAULT '',
  `hitCount` tinyint(3) NOT NULL DEFAULT '0',
  `def` varchar(255) NOT NULL DEFAULT '',
  `magdef` varchar(255) NOT NULL DEFAULT '',
  `evade` float(10,4) NOT NULL DEFAULT '0.0000',
  `pierce` float(10,4) NOT NULL DEFAULT '0.0000',
  `adddam` float(10,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `cittas`;
CREATE TABLE `cittas` (
  `id` smallint(5) NOT NULL,
  `name` varchar(255) NOT NULL,
  `needsoul` smallint(5) NOT NULL DEFAULT '0',
  `effectid` smallint(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `citta_effect`;
CREATE TABLE `citta_effect` (
  `id` smallint(5) NOT NULL,
  `strength` varchar(10) NOT NULL DEFAULT '0',
  `physique` varchar(10) NOT NULL DEFAULT '0',
  `agility` varchar(10) NOT NULL DEFAULT '0',
  `intelligence` varchar(10) NOT NULL DEFAULT '0',
  `will` varchar(10) NOT NULL DEFAULT '0',
  `soul` varchar(10) NOT NULL DEFAULT '0',
  `aura` varchar(10) NOT NULL DEFAULT '0',
  `auraMax` varchar(10) NOT NULL DEFAULT '0',
  `attack` varchar(64) NOT NULL DEFAULT '0',
  `mag_attack` varchar(64) NOT NULL DEFAULT '0',
  `defend` varchar(64) NOT NULL DEFAULT '0',
  `mag_defend` varchar(64) NOT NULL DEFAULT '0',
  `hp` varchar(64) NOT NULL DEFAULT '0',
  `skill` varchar(255) NOT NULL DEFAULT '0',
  `action` float(10,4) NOT NULL DEFAULT '0.0000',
  `hitrate` float(10,4) NOT NULL DEFAULT '0.0000',
  `evade` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical_dmg` float(10,4) NOT NULL DEFAULT '0.0000',
  `pierce` float(10,4) NOT NULL DEFAULT '0.0000',
  `counter` float(10,4) NOT NULL DEFAULT '0.0000',
  `mag_res` float(10,4) NOT NULL DEFAULT '0.0000',
  `practice` float(10,4) NOT NULL DEFAULT '0.0000',
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
  `capacity` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  `skill` varchar(255) NOT NULL DEFAULT '',
  `npc_weapon` int(10) unsigned NOT NULL DEFAULT '0',
  `strength` int(5) NOT NULL DEFAULT '10',
  `physique` int(5) NOT NULL DEFAULT '10',
  `agility` int(5) NOT NULL DEFAULT '10',
  `intelligence` int(5) NOT NULL DEFAULT '10',
  `will` int(5) NOT NULL DEFAULT '10',
  `soul` int(5) NOT NULL DEFAULT '10',
  `aura` int(5) NOT NULL DEFAULT '10',
  `auraMax` int(5) NOT NULL DEFAULT '10',
  `tough` int(5) NOT NULL DEFAULT '10',
  `attack` int(5) NOT NULL DEFAULT '10',
  `mag_attack` int(5) NOT NULL DEFAULT '10',
  `defend` int(5) NOT NULL DEFAULT '10',
  `mag_defend` int(5) NOT NULL DEFAULT '10',
  `hp` int(10) NOT NULL DEFAULT '20',
  `action` int(5) NOT NULL DEFAULT '6000',
  `peerless` int(10) NOT NULL DEFAULT '20',
  `hitrate` float(10,4) NOT NULL DEFAULT '90.0000',
  `evade` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical` float(10,4) NOT NULL DEFAULT '0.0000',
  `critical_dmg` float(10,4) NOT NULL DEFAULT '0.0000',
  `pierce` float(10,4) NOT NULL DEFAULT '0.0000',
  `counter` float(10,4) NOT NULL DEFAULT '0.0000',
  `mag_res` float(10,4) NOT NULL DEFAULT '0.0000',
  `extraPos` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

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

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed
