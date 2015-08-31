DROP TABLE IF EXISTS `endconstantly_kill`; 
CREATE TABLE `endconstantly_kill` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家Id',
    `fighterId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '自己的战将id',
    `peerId`   bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '被终结的玩家Id',
    `peerFighterId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT  '对方战将Id',
    `endkillNum` int(10)  NOT NULL DEFAULT '0' COMMENT '连杀数量',
    PRIMARY KEY (`playerId`,`fighterId`,`peerId`,`peerFighterId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
