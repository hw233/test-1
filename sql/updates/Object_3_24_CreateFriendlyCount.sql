CREATE TABLE IF NOT EXISTS `friendlyCount` (
    `playerId` bigint(20) unsigned NOT NULL,
    `friendId` bigint(20) unsigned NOT NULL,
    `value` int(10) unsigned NOT NULL DEFAULT '0',
    `isBrother` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `time` int(10) unsigned NOT NULL DEFAULT '0',
    `cost` int(10) unsigned NOT NULL DEFAULT '0',
    `wait` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `ybTime` int(10) unsigned NOT NULL DEFAULT '0',
    `ybCount` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `clearTime` int(10) unsigned NOT NULL DEFAULT '0',
    `task1` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `task2` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `task3` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `task4` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `task5` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `task6` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `friendId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


