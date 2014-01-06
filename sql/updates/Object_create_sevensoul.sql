CREATE TABLE IF NOT EXISTS `player_sevensoul` (
    `playerId` bigint(20) unsigned NOT NULL,
    `petId` int(10) unsigned NOT NULL,
    `soulId` tinyint(3) unsigned NOT NULL,
    `soulLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `skillIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `petId`, `soulId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

