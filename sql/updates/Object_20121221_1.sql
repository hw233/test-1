CREATE TABLE IF NOT EXISTS `snow` (                       
    `playerId` bigint(20) unsigned NOT NULL,  
    `lover` bigint(20) unsigned NOT NULL,     
    `bind` tinyint(3) unsigned NOT NULL,      
    `score` int(10) unsigned NOT NULL,        
    PRIMARY KEY (`playerId`)                  
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
