CREATE TABLE IF NOT EXISTS `skill_grade` (
    `playerId` bigint(20) unsigned NOT NULL,
    `fighterId` int(10) unsigned NOT NULL,
    `skillId` smallint(5) unsigned NOT NULL,
    `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `fighterId`, `skillId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

