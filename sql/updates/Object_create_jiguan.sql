CREATE TABLE IF NOT EXISTS `player_jiguanshu` (
    `playerId` int(10) unsigned NOT NULL DEFAULT '0',
    `curLvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `curExp` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `player_jiguanyu` (
    `playerId` int(10) unsigned NOT NULL DEFAULT '0',
    `jiguanId` int(10) unsigned NOT NULL DEFAULT '0',
    `pos` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`, `jiguanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `player_tuzhi` (
    `playerId` int(10) unsigned NOT NULL DEFAULT '0',
    `tuzhiId` int(10) unsigned NOT NULL DEFAULT '0',
    `curProficient` tinyint(3) unsigned NOT NULL DEFAULT '0',
   PRIMARY KEY(`playerId`, `tuzhiId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
