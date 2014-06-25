
CREATE TABLE IF NOT EXISTS `happyXXL` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `num`  tinyint(3) unsigned NOT NULL DEFAULT '0',
    `map` varchar(256) NOT NULL DEFAULT '',
    PRIMARY KEY(`playerId`,`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
