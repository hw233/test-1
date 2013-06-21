DROP TABLE IF EXISTS `soul_skill_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `soul_skill_template` (
  `id` tinyint(3) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `level` tinyint(3) unsigned NOT NULL,
  `itemId` int(10) unsigned NOT NULL DEFAULT 0,
  `value` float(7,2) unsigned NOT NULL,
  PRIMARY KEY (`id`, `level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


