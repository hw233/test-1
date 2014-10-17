CREATE TABLE `horcruxAttr` (                       
    `id` int(10) unsigned NOT NULL DEFAULT '0',        
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `itemId` int(10) unsigned NOT NULL DEFAULT '0',
    `value1` int(10) unsigned NOT NULL DEFAULT '0',        
    `value2` int(10) unsigned NOT NULL DEFAULT '0',        
    `value3` int(10) unsigned NOT NULL DEFAULT '0',        
    `value4` int(10) unsigned NOT NULL DEFAULT '0',        
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
