
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_statue_template` (
    `level` int(10) unsigned NOT NULL DEFAULT 0,
    `needExp` bigint(20) unsigned NOT NULL DEFAULT 0,
    `consumeExp` bigint(20) unsigned NOT NULL DEFAULT 0,
    `exHp` bigint(20) unsigned NOT NULL DEFAULT 0,
    `exAttack` bigint(20) unsigned NOT NULL DEFAULT 0,
    `exDefend` bigint(20) unsigned NOT NULL DEFAULT 0,
    `exMagAtk` bigint(20) unsigned NOT NULL DEFAULT 0,
    `exMagDef` bigint(20) unsigned NOT NULL DEFAULT 0,
    `exAction` bigint(20) unsigned NOT NULL DEFAULT 0,
    `exHitRate` bigint(20) unsigned NOT NULL DEFAULT 0,
     PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_copy_monster_template` (
    `level` int(10) unsigned NOT NULL DEFAULT 0,
    `appearRound` int(10) unsigned NOT NULL DEFAULT 0,
    `npcId` bigint(20) unsigned NOT NULL DEFAULT 0,
    `npcCount` smallint(5) unsigned NOT NULL DEFAULT 0,
    `npcRouteCount` smallint(5) unsigned NOT NULL DEFAULT 0,
    `npcValue` smallint(5) unsigned NOT NULL DEFAULT 0,
    `monsterType` smallint(5) unsigned NOT NULL DEFAULT 0
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_copy_template` (
    `level` int(10) unsigned NOT NULL DEFAULT 0,
    `monsterWaveCount` int(10) unsigned NOT NULL DEFAULT 0,
    `expOutput` bigint(20) unsigned NOT NULL DEFAULT 0,
    `minPlayer` int(10) unsigned NOT NULL DEFAULT 0,
    `maxPlayer` int(10) unsigned NOT NULL DEFAULT 0,
    `spotMaxPlayer` int(10) unsigned NOT NULL DEFAULT 0,
    `homeHp` bigint(20) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

