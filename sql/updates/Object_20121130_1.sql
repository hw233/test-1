CREATE TABLE IF NOT EXISTS `recharge_item` (               
    `player_id` int(20) NOT NULL DEFAULT '0',  
    `item_id` int(11) NOT NULL DEFAULT '0',    
    `num` int(11) DEFAULT NULL,                
    PRIMARY KEY (`player_id`,`item_id`)        
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

