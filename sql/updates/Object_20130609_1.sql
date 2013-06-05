CREATE TABLE IF NOT EXISTS `fighter_xingchen` (
    `fighterId` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `curVal` int(10) unsigned NOT NULL DEFAULT 0,
    `gem1` smallint(6) unsigned NOT NULL DEFAULT 0,
    `gem2` smallint(6) unsigned NOT NULL DEFAULT 0,
    `gem3` smallint(6) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY(`fighterId`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

