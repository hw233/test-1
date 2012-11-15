CREATE TABLE `strengthenData` (
   `playerId` bigint(20) unsigned NOT NULL,
   `overTime` int(10) unsigned NOT NULL,
   `souls`  int(10) unsigned NOT NULL,
   `soulId` tinyint(3) unsigned NOT NULL,
   `flags`  varchar(255) NOT NULL DEFAULT '',
   `greenBox`  varchar(255) NOT NULL DEFAULT '',
   `blueBox`  varchar(255) NOT NULL DEFAULT '',
   `purpleBox`  varchar(255) NOT NULL DEFAULT '',
   `orangeBox`  varchar(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `days_rank` (
    `player_id` bigint(20) unsigned NOT NULL,
    `athletics` char(100) NOT NULL DEFAULT '', 
    `level` char(100) NOT NULL DEFAULT '', 
    `money` char(100) NOT NULL DEFAULT '', 
    `pk` char(100) NOT NULL DEFAULT '',
    `battle` char(100) NOT NULL DEFAULT '',
    PRIMARY KEY (`player_id`),
    UNIQUE KEY `unq` (`player_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
