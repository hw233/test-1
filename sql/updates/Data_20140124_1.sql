CREATE TABLE `clan_building_template` (
      `level` smallint(6) NOT NULL COMMENT '建筑等级',
      `needExp` int(10) DEFAULT NULL COMMENT '该等级所需经验值',
      `phyAtkValue` int(10) DEFAULT NULL COMMENT '物攻属性加成值',
      `magAtkValue` int(10) DEFAULT NULL COMMENT '法攻属性加成值',
      `actionValue` int(10) DEFAULT NULL COMMENT '身法属性加成值',
      `hpValue` int(10) DEFAULT NULL COMMENT '生命属性加成值',
      `oracleValue` int(10) DEFAULT NULL COMMENT '神谕塔对其他建筑的等级限制',
      PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='帮派建筑属性等级表';
