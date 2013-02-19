/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dreamer_template` (
    `index` int(10) unsigned NOT NULL DEFAULT 0,
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `floor` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `maxX` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `maxY` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `gridCount` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `timeConsume` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `typeCount` varchar(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

