--DROP TABLE IF EXISTS `clan_buildings`;
CREATE TABLE `clan_buildings` (
      `clanId` bigint(20) NOT NULL COMMENT '帮派ID',
      `fairylandEnergy` bigint(20) DEFAULT '0' COMMENT '仙界元气',
      `phyAtkLevel` smallint(6) DEFAULT '0' COMMENT '物攻建筑等级',
      `magAtkLevel` smallint(6) DEFAULT '0' COMMENT '法攻建筑等级',
      `actionLevel` smallint(6) DEFAULT '0' COMMENT '身法建筑等级',
      `hpLevel` smallint(6) DEFAULT '0' COMMENT '生命建筑等级',
      `oracleLevel` smallint(6) DEFAULT '0' COMMENT '神谕塔建筑等级',
      `updateTime` int(10) DEFAULT '0' COMMENT '数据更新时间',
      PRIMARY KEY (`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='帮派建筑';

