alter table `athletics_rank` drop column `boxColor`;
alter table `athletics_rank` drop column `boxType`;
alter table `athletics_rank` drop column `boxCount`;
alter table `athletics_rank` drop column `boxFlushTime`;

alter table `athletics_rank` add column `prestige` int(10) unsigned NOT NULL DEFAULT '0' after `challengeTime`;
alter table `athletics_rank` add column `bewinstreak` smallint(3) unsigned NOT NULL DEFAULT '0' after `winStreak`;
alter table `athletics_rank` add column `failstreak` smallint(3) unsigned NOT NULL DEFAULT '0' after `bewinstreak`;
alter table `athletics_rank` add column `befailstreak` smallint(3) unsigned NOT NULL DEFAULT '0' after `failstreak`;
alter table `athletics_rank` add column `oldrank` int(10) unsigned NOT NULL DEFAULT '0' after `befailstreak`;
alter table `athletics_rank` add column `first4rank` int(10) unsigned NOT NULL DEFAULT '0' after `oldrank`;
alter table `athletics_rank` add column `extrachallenge` int(10) unsigned NOT NULL DEFAULT '0' after `first4rank`;


alter table `athletics_record` drop column `awardType`;
alter table `athletics_record` drop column `awardAtkerCount`;


-- Table structure for table `athletics_event`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `athletics_event` (
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


alter table `player` add column `dungeonCnt` tinyint(3) unsigned NOT NULL DEFAULT '0' after `wallow`;
alter table `player` add column `dungeonEnd` int(10) unsigned NOT NULL DEFAULT '0' after `dungeonCnt`;

