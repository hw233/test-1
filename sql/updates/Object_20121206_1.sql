CREATE TABLE IF NOT EXISTS `copy_front_win` ( 
    `playerId` bigint(20) NOT NULL DEFAULT '0',
    `posOrig` tinyint(3) NOT NULL DEFAULT '0',
    `posPut` tinyint(3) NOT NULL DEFAULT '0',
    `itemId` int(10) unsigned NOT NULL DEFAULT '0',
    `ratio` mediumint(5) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`playerId`, `posOrig`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

