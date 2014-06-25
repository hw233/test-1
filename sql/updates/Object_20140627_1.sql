CREATE TABLE IF NOT EXISTS `player_gear` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `gearId` int(10) unsigned NOT NULL DEFAULT '0',
    `mark` tinyint(3) unsigned NOT NULL DEFAULT '0',
   PRIMARY KEY(`playerId`, `gearId`, `mark`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

