CREATE TABLE IF NOT EXISTS `fairyPet_sanhun` (
    `fairyPetId` int(10) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `sanhunId` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `curLvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `fairyPetId`, `sanhunId`),
    KEY `fairyPetId` (`fairyPetId`),
    KEY `playerId` (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

