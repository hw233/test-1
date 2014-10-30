CREATE TABLE IF NOT EXISTS `fighter_horcrux` (
    `fighterId` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `criticaldefExp` int(10) unsigned NOT NULL DEFAULT 0,
    `piercedefExp` int(10) unsigned NOT NULL DEFAULT 0,
    `counterdefExp` int(10) unsigned NOT NULL DEFAULT 0,
    `attackpierceExp` int(10) unsigned NOT NULL DEFAULT 0,
    `lingshi_exp` int(10) unsigned NOT NULL DEFAULT 0,
    `lingshi_exp2` int(10) unsigned NOT NULL DEFAULT 0,
    `lingshi_exp3` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY(`fighterId`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
