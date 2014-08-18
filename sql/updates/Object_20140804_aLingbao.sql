CREATE TABLE IF NOT EXISTS `fighter_lingbaoLevel` (
    `fighterId` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `lingbaoType` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `enLevel` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY(`fighterId`, `playerId`,`lingbaoType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
