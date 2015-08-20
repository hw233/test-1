DROP TABLE IF EXISTS `arenaLayout`; 
CREATE TABLE `arenaLayout` (
    `playerId` bigint(20) unsigned NOT NULL,
    `index` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '战役Id',
    `fighterId`  int(10) NOT NULL DEFAULT '0' COMMENT '战术发生时间',
    PRIMARY KEY (`playerId`,`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
