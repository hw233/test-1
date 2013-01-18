
CREATE TABLE IF NOT EXISTS `lingbaoattr` (                       
    `id` int(10) unsigned NOT NULL,        
    `tongling` tinyint(3) unsigned NOT NULL,      
    `lbcolor` tinyint(3) unsigned NOT NULL,      
    `types` varchar(256) NOT NULL DEFAULT '',
    `values` varchar(256) NOT NULL DEFAULT '',
    `skills` varchar(256) NOT NULL DEFAULT '',
    `factors` varchar(256) NOT NULL DEFAULT '',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `lingbaosmelt` (                       
    `playerId` bigint(20) unsigned NOT NULL,  
    `gujiId` smallint(4) unsigned NOT NULL,
    `itemId` smallint(4) unsigned NOT NULL,
    `bind` tinyint(3) unsigned NOT NULL,      
    `value` int(10) unsigned NOT NULL,        
    `maxValue` int(10) unsigned NOT NULL,        
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



