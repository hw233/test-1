
CREATE TABLE IF NOT EXISTS `worldCup` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `num`  tinyint(3) unsigned NOT NULL DEFAULT '0',
    `count1` int(10) unsigned NOT NULL DEFAULT '0',
    `count2` int(10) unsigned NOT NULL DEFAULT '0',
    `count3` int(10) unsigned NOT NULL DEFAULT '0',
    `result` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`,`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
