 /*!40101 SET @saved_cs_client     = @@character_set_client */;
 /*!40101 SET character_set_client = utf8 */;
 CREATE TABLE IF NOT EXISTS `towndeamon_monster` (
       `level` smallint(3) unsigned NOT NULL,
       `npcId` int(3) unsigned NOT NULL,
       `itemId` int(10) unsigned NOT NULL,
       `itemNum` int(10) NOT NULL,
       PRIMARY KEY (`level`)
     ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
     /*!40101 SET character_set_client = @saved_cs_client */;
