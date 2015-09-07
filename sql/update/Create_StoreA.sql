DROP TABLE IF EXISTS `storeA`; 
CREATE TABLE `storeA` (                                                                                                                                                    
    `playerId` bigint(20) unsigned NOT NULL COMMENT '玩家Id',
    `pageId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '页面id',
    `index`  tinyint(10) NOT NULL DEFAULT '0' COMMENT '位置Id',
    `limitCount`  int(10) NOT NULL DEFAULT '0' COMMENT '剩余的物品数量',
    PRIMARY KEY (`playerId`,`pageId`,`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
