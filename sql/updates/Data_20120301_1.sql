/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `spirit_attr` (
  `level` smallint(3) unsigned NOT NULL,
  `attack` float(5,2) unsigned NOT NULL,
  `defend` float(5,2) unsigned NOT NULL,
  `critical` float(5,2) unsigned NOT NULL,
  `pierce` float(5,2) unsigned NOT NULL,
  `action` float(5,2) unsigned NOT NULL,
  `tough` float(5,2) unsigned NOT NULL,
  `criticaldmg` float(5,2) unsigned NOT NULL,
  `hp` float(5,2) unsigned NOT NULL,
  PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


