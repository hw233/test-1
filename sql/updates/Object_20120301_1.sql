
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `equipment_spirit` (
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

