CREATE TABLE IF NOT EXISTS `player_keyin` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `keyinId` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `curLvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `curValue` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `keyinId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
