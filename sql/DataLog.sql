-- MySQL dump 10.13  Distrib 5.1.53, for pc-linux-gnu (x86_64)
--
-- Host: localhost    Database: datalog
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
-- Table structure for table `register_states`
--

SET NAMES UTF8;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `register_states` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `player_name` varchar(255) NOT NULL,
  `level` tinyint(3) unsigned NOT NULL,
  `platform` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `reg_time` int(10) unsigned NOT NULL,
  PRIMARY KEY (`server_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `login_states`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `login_states` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `login_time` int(10) unsigned NOT NULL,
  `login_ip` varchar(255) NOT NULL,
  `logout_time` int(10) unsigned NOT NULL,
  INDEX up_login_status (`server_id`, `player_id`, `login_time`),
  INDEX up_player_login (`player_id`, `login_time`),
  INDEX up_login_time (`login_time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mailitem_histories`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `mailitem_histories` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `mail_id` int(10) unsigned NOT NULL,
  `mail_type` tinyint(3) unsigned NOT NULL,
  `title` varchar(255) NOT NULL,
  `content_text` varchar(8192) NOT NULL DEFAULT '',
  `content_item` varchar(255) NOT NULL,
  `receive_time` int(10) unsigned NOT NULL,
  `status` tinyint(3) unsigned NOT NULL,
  `delete_time` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`server_id`, `player_id`, `mail_id`),
  INDEX up_mail_status (`server_id`, `mail_id`, `status`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `topup_num`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `topup_num` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `topup_gold` int(10) unsigned NOT NULL,
  `rand_num` tinyint(3) unsigned NOT NULL,
  `topup_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `consume_gold`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_gold` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `consume_demon`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_demon` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `id` int(10) unsigned NOT NULL,
  `num` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `consume_achievement`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_achievement` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `consume_prestige`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_prestige` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `consume_coin`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_coin` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `consume_tael`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_tael` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `consume_coupon`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_coupon` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `item_courses`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `item_courses` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `from_to` tinyint(3) unsigned NOT NULL,
  `happened_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `equip_courses`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `equip_courses` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `template_id` int(10) unsigned NOT NULL,
  `equip_id` int(10) unsigned NOT NULL,
  `from_to` tinyint(3) unsigned NOT NULL,
  `happened_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `item_histories`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `item_histories` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `use_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `enchant_histories`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `enchant_histories` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `equip_id` int(10) unsigned NOT NULL,
  `template_id` int(10) unsigned NOT NULL,
  `enchant_level` tinyint(3) unsigned NOT NULL,
  `enchant_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dungeon_statistics`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `dungeon_statistics` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `dungeon_id` tinyint(3) unsigned NOT NULL,
  `this_day` int(10) unsigned NOT NULL,
  `pass_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `online_situations`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `online_situations` (
  `server_id` int(10) unsigned NOT NULL,
  `divtime` int(10) unsigned NOT NULL,
  `num` int(10) unsigned NOT NULL,
  INDEX server(`server_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `country_battle`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `country_battle` (
  `server_id` int(10) unsigned NOT NULL,
  `total_achievement1` int(10) unsigned NOT NULL,
  `total_players1` int(10) unsigned NOT NULL,
  `total_achievement2` int(10) unsigned NOT NULL,
  `total_players2` int(10) unsigned NOT NULL,
  `total_achievement3` int(10) unsigned NOT NULL,
  `total_players3` int(10) unsigned NOT NULL,
  `max_player1` int(10) unsigned NOT NULL,
  `max_achievement1` int(10) unsigned NOT NULL,
  `max_player2` int(10) unsigned NOT NULL,
  `max_achievement2` int(10) unsigned NOT NULL,
  `max_player3` int(10) unsigned NOT NULL,
  `max_achievement3` int(10) unsigned NOT NULL,
  `created_at` int(10) unsigned NOT NULL,
  `num` int(10) unsigned NOT NULL,
  INDEX server(`server_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `sales`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `sales` (
  `server_id` int(10) unsigned NOT NULL,
  `sale_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `price_type` int(10) unsigned NOT NULL,
  `price` int(10) unsigned NOT NULL,
  `item` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `sale_time` int(10) unsigned NOT NULL,
  `is_cancel` tinyint(1) NOT NULL DEFAULT 0,
  `purchaser` bigint(20) unsigned NOT NULL DEFAULT 0,
  `purchase_time` int(10) unsigned NOT NULL DEFAULT 0,
  INDEX server(`server_id`, `sale_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `upgrade_histories`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `upgrade_histories` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `equip_id` int(10) unsigned NOT NULL,
  `template_id` int(10) unsigned NOT NULL,
  `equip_rank` tinyint(3) unsigned NOT NULL,
  `upgrade_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `lorder_histories`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `lorder_histories` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `equip_id` int(10) unsigned NOT NULL,
  `template_id` int(10) unsigned NOT NULL,
  `equip_maxrank` tinyint(3) unsigned NOT NULL,
  `upgrade_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_arena` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_clan_proffer` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `consume_pet` (
  `server_id` int(10) unsigned NOT NULL,
  `player_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `consume_type` int(10) unsigned NOT NULL,
  `item_num` int(10) unsigned NOT NULL,
  `expenditure` int(10) unsigned NOT NULL,
  `consume_time` int(10) unsigned NOT NULL,
  INDEX server_player (`server_id`, `player_id`),
  INDEX server_player_item (`server_id`, `player_id`, `item_id`),
  INDEX server_player_type (`server_id`, `player_id`, `consume_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

-- Dump completed
