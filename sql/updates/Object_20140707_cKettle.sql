
CREATE TABLE IF NOT EXISTS `kettle` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0', 
    `num`  tinyint(3) unsigned NOT NULL DEFAULT '0',  
    `history` varchar(1024) NOT NULL DEFAULT '',  
    `occupy` varchar(256) NOT NULL DEFAULT '',   
    PRIMARY KEY(`playerId`,`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
