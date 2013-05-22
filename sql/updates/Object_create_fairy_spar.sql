CREATE TABLE IF NOT EXISTS `fairy_spar` (
    `playerId` bigint(20) unsigned NOT NULL,
    `atk` int(10) unsigned NOT NULL DEFAULT '0',
    `magAtk` int(10) unsigned NOT NULL DEFAULT '0',
    `phy` int(10) unsigned NOT NULL DEFAULT '0',
    `element1` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `element2` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `element3` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `element4` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `element5` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `complexPercent` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `curMark` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `breakoutCnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

