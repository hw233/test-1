CREATE TABLE IF NOT EXISTS `friendlyCount` (
    `playerId` bigint(20) unsigned NOT NULL,
    `friendId` bigint(20) unsigned NOT NULL,
    `value` int(10) unsigned NOT NULL DEFAULT '0',
    `isBrother` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `cost` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `friendId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


