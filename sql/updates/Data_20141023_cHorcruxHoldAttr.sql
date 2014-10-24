DROP TABLE IF EXISTS `horcruxHoldAttr1`;
CREATE TABLE `horcruxHoldAttr1` (
    `val` int(10) unsigned NOT NULL DEFAULT 0,
    `up`  int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`val`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `horcruxHoldAttr2`;
CREATE TABLE `horcruxHoldAttr2` (
    `val` int(10) unsigned NOT NULL DEFAULT 0,
    `up` float(10,2) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`val`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
--
