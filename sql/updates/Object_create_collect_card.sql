CREATE TABLE IF NOT EXISTS `collect_card` (                       
    `playerId` bigint(20) unsigned NOT NULL,  
    `id` tinyint(3) unsigned NOT NULL,
    `partCnt1` smallint(6) unsigned NOT NULL,
    `partCnt2` smallint(6) unsigned NOT NULL,
    `partCnt3` smallint(6) unsigned NOT NULL,
    `partCnt4` smallint(6) unsigned NOT NULL,
    `partCnt5` smallint(6) unsigned NOT NULL,
    `partCnt6` smallint(6) unsigned NOT NULL,
    `partCnt7` smallint(6) unsigned NOT NULL,
    `partCnt8` smallint(6) unsigned NOT NULL,
    `partCnt9` smallint(6) unsigned NOT NULL,
    `alreadyCnt` smallint(6) unsigned NOT NULL,
    PRIMARY KEY (`playerId`, `id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

