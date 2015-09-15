DROP TABLE IF EXISTS `arenaBrp`; 
CREATE TABLE `arenaBrp` (
    `playerId` bigint(20) unsigned NOT NULL,
    `battleId` int(10) unsigned NOT NULL,
    `name` varchar(255) NOT NULL,
    `res` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '胜负',
    /*`playerId` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '对象编号',*/
    `index` int(10) unsigned NOT NULL COMMENT '排名',
    `power` int(10) unsigned NOT NULL COMMENT '战斗力',
    PRIMARY KEY (`battleId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
