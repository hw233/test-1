DROP TABLE IF EXISTS `constantly_kill`; 
CREATE TABLE `constantly_kill` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家Id',
    `fighterId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT  '战将Id',
    `killNum` int(10)  NOT NULL DEFAULT '0' COMMENT '连杀数量',
    PRIMARY KEY (`playerId`,`fighterId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
