DROP TABLE IF EXISTS `player_vip`;
CREATE TABLE `player_vip` (                                                                                                                                                       `level` tinyint(3) NOT NULL DEFAULT '0' COMMENT 'vip等级',
    `needCoin`   int(10) NOT NULL DEFAULT '0' COMMENT '所需充值的数量',
    `itemIds`  varchar(255) NOT NULL,
    `itemNums`  varchar(255) NOT NULL,
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
