CREATE TABLE `job_hunter` (
    `playerId` bigint(20) unsigned NOT NULL COMMENT '玩家Id',
    `fighterList` varchar(1024) NOT NULL DEFAULT '' COMMENT '墨家散仙列表',
    `mapInfo` varchar(1024) NOT NULL DEFAULT '' COMMENT '寻墨地图信息',
    `progress` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT '游戏进程状态',
    `posX` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'X坐标值',
    `posY` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Y坐标值',
    `earlyPosX` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT '上一步X坐标值',
    `earlyPosY` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT '上一步Y坐标值',
    `stepCount` int(10) unsigned NOT NULL DEFAULT 0 COMMENT '已使用步数',
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

