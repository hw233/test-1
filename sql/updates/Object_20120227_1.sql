 CREATE TABLE IF NOT EXISTS `towndeamon_player` (
       `deamonLevel` smallint(3) unsigned NOT NULL,
       `curLevel` smallint(3) unsigned NOT NULL,
       `maxLevel` smallint(3) unsigned NOT NULL,
       `playerId` bigint(20) unsigned NOT NULL,
       `startTime` int(10) unsigned NOT NULL,
       `accTime` int(10) unsigned NOT NULL,
       `accLen` int(10) unsigned NOT NULL,
       `accAwards` int(10) unsigned NOT NULL,
       `vitalityTime` int(10) unsigned NOT NULL,
       `vitality` int(10) unsigned NOT NULL,
       `spirit` int(10) unsigned NOT NULL,
       `challengeTime` int(10) unsigned NOT NULL,
       `itemId` int(10) unsigned NOT NULL DEFAULT 0,
       `itemNum` int(10) unsigned NOT NULL DEFAULT 0,
       `quitLevel` smallint(3) unsigned NOT NULL DEFAULT 0,
       `attacker` bigint(20) unsigned NOT NULL DEFAULT 0,
       PRIMARY KEY (`playerId`)
     ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

