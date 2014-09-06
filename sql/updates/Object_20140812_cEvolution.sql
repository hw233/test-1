
CREATE TABLE IF NOT EXISTS `fighter_evolution` (
    `fighterId` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `process` int(10) unsigned NOT NULL DEFAULT 0,
    `award` int(10) unsigned NOT NULL DEFAULT 0,
    `task9` varchar(256) NOT NULL DEFAULT '',  
    `rTime` int(10) unsigned NOT NULL DEFAULT 0,
    `success`  tinyint(3) unsigned NOT NULL DEFAULT '0',  
    PRIMARY KEY(`fighterId`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

