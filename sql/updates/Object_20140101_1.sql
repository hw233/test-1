CREATE TABLE IF NOT EXISTS `player_petteamcopy` (
    `playerId` bigint(20) unsigned NOT NULL,
    `copyId` tinyint(3) unsigned NOT NULL,
    `type` tinyint(3) unsigned NOT NULL,
    `npcGroup1Id` int(10) unsigned NOT NULL DEFAULT '0',
    `npcGroup2Id` int(10) unsigned NOT NULL DEFAULT '0',
    `npcGroup3Id` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`playerId`,`copyId`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
