
CREATE TABLE `skillstrengthenprob` (
    `id` smallint(5) unsigned NOT NULL DEFAULT 0,
    `prob1` int(10) unsigned NOT NULL DEFAULT 0,
    `prob2` int(10) unsigned NOT NULL DEFAULT 0,
    `prob3` int(10) unsigned NOT NULL DEFAULT 0,
    `prob4` int(10) unsigned NOT NULL DEFAULT 0,
    `prob5` int(10) unsigned NOT NULL DEFAULT 0,
    `prob6` int(10) unsigned NOT NULL DEFAULT 0,
    `prob7` int(10) unsigned NOT NULL DEFAULT 0,
    `prob8` int(10) unsigned NOT NULL DEFAULT 0,
    `prob9` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `skillstrengthen` (
    `id` smallint(5) unsigned NOT NULL DEFAULT 0,
    `exp1` int(10) unsigned NOT NULL DEFAULT 0,
    `exp2` int(10) unsigned NOT NULL DEFAULT 0,
    `exp3` int(10) unsigned NOT NULL DEFAULT 0,
    `exp4` int(10) unsigned NOT NULL DEFAULT 0,
    `exp5` int(10) unsigned NOT NULL DEFAULT 0,
    `exp6` int(10) unsigned NOT NULL DEFAULT 0,
    `exp7` int(10) unsigned NOT NULL DEFAULT 0,
    `exp8` int(10) unsigned NOT NULL DEFAULT 0,
    `exp9` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skillstrengthen_skill` (
  `id` smallint(6) unsigned NOT NULL DEFAULT 0,
  `name` varchar(255) NOT NULL,
  `effect` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skillstrengthen_effect` (
  `id` smallint(6) unsigned NOT NULL DEFAULT 0,
  `cond` tinyint(3) NOT NULL,
  `target` tinyint(3) NOT NULL,
  `prob` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  `area` tinyint(3) NOT NULL,
  `factor` varchar(255) NOT NULL,
  `last` tinyint(3) NOT NULL,
  `type` smallint(6) unsigned NOT NULL DEFAULT 0,
  `value` float(5,2) unsigned NOT NULL DEFAULT '1.00',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


