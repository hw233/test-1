/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `lbskills` (
  `id` smallint(5) unsigned NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `lbtype` tinyint(3) NOT NULL DEFAULT '0',
  `level` tinyint(3) NOT NULL DEFAULT '0',
  `target` tinyint(3) NOT NULL DEFAULT '0',
  `prob` float(10,4) NOT NULL DEFAULT '0.0000',
  `cond` smallint(5) NOT NULL DEFAULT '0',
  `cond2` smallint(5) NOT NULL DEFAULT '0',
  `area` tinyint(3) NOT NULL DEFAULT '0',
  `factor` varchar(255) NOT NULL DEFAULT '',
  `last` smallint(5) NOT NULL DEFAULT '0',
  `cd` smallint(5) NOT NULL DEFAULT '0',
  `eftype` smallint(5) NOT NULL DEFAULT '0',
  `minfactor` smallint(5) NOT NULL DEFAULT '0',
  `efvalue` smallint(5) NOT NULL DEFAULT '0',
  `battlepoint` smallint(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


