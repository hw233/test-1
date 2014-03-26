DROP TABLE IF EXISTS `fighter_xinmo`;
CREATE TABLE IF NOT EXISTS `fighter_xinmo` (
    `fighterId` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `xinmolev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `curVal` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY(`fighterId`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

