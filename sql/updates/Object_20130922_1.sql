CREATE TABLE IF NOT EXISTS `player_zhenwei` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `keyId` smallint(6) unsigned NOT NULL DEFAULT '0',
    `mark` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `keyId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
