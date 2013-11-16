CREATE TABLE IF NOT EXISTS `guanggun` (
    `playerId` bigint(20) unsigned NOT NULL,
    `status` tinyint(3) unsigned NOT NULL,
    `playerId1` bigint(20) unsigned NOT NULL,
    `playerId2` bigint(20) unsigned NOT NULL,
    `pos` tinyint(3) unsigned NOT NULL DEFAULT '1' ,
    `score` int(10) unsigned NOT NULL,
    `task` tinyint(3) unsigned NOT NULL,
    `tasknum` tinyint(3) unsigned NOT NULL,
    `taskCom` tinyint(3) unsigned NOT NULL,
    `counts` int(10) unsigned NOT NULL,
    PRIMARY KEY(`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
