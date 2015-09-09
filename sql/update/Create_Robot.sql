DROP TABLE IF EXISTS `robot`;
CREATE TABLE `robot` (
    `index` int(10) NOT NULL DEFAULT '1' COMMENT '机器人编号',
    `front`   tinyint(10) NOT NULL DEFAULT '0' COMMENT '布阵',
    `fgt1`   int(10) NOT NULL DEFAULT '0' COMMENT '战将1',
    `fgt2`   int(10) NOT NULL DEFAULT '0' COMMENT '战将2',
    `fgt3`   int(10) NOT NULL DEFAULT '0' COMMENT '战将3',
    `fgt4`   int(10) NOT NULL DEFAULT '0' COMMENT '战将4',
    `fgt5`   int(10) NOT NULL DEFAULT '0' COMMENT '战将5',
    PRIMARY KEY (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
