CREATE TABLE   IF NOT EXISTS  `activityData`
(`playerId` bigint(20) unsigned NOT NULL,
    `overTime` int(10) unsigned NOT NULL,
    `awardId`  int(10) unsigned NOT NULL,
    `point`   int(10) unsigned NOT NULL, 
    `award`   int(10) unsigned NOT NULL,
    `flags`    varchar(255) NOT NULL DEFAULT '',
     PRIMARY KEY (`playerId`) 
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;
