
DROP TABLE IF EXISTS `jiguanshu`;
CREATE TABLE `jiguanshu` (
    `jgshuLvl` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `totalNeedExp` int(10) unsigned NOT NULL DEFAULT 0,
    `needExp` int(10) unsigned NOT NULL DEFAULT 0,
    `attrValue1` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValue2` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValue3` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValue4` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`jgshuLvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `jiguanyu`;
CREATE TABLE `jiguanyu` (
    `jiguanyuId` int(10) unsigned NOT NULL DEFAULT 0,
    `mark` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `quality` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `attrType` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `attrValue` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `element` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `molding` varchar(255) NOT NULL DEFAULT '',
    `moldingType` varchar(255) NOT NULL DEFAULT '',
    `skillId` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`jiguanyuId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `tuzhi`;
CREATE TABLE `tuzhi` (
    `tuzhiId` int(10) unsigned NOT NULL DEFAULT 0,
    `mark` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `quality` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `needjgsLvl` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `jiguanyuId` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`tuzhiId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


