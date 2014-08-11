CREATE TABLE IF NOT EXISTS `fighter_lingbaoFall` (
    `fighterId` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `fall` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY(`fighterId`, `playerId`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
