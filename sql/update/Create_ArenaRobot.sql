DROP TABLE IF EXISTS `arenaRobot`; 
CREATE TABLE `arenaRobot` (
    `index` int(10) unsigned NOT NULL,
    `firstIndex` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '机器人初始编号',
    `robotId`  int(10) NOT NULL  COMMENT '机器人编号',
    PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
