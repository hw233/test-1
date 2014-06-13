DROP TABLE IF EXISTS `gear`;
CREATE TABLE `gear` (
    `partId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `needMoneyA` int(10) unsigned NOT NULL DEFAULT 0,
    `needMoneyB` int(10) unsigned NOT NULL DEFAULT 0,
    `needMoneyC` int(10) unsigned NOT NULL DEFAULT 0,
    `needMoneyD` int(10) unsigned NOT NULL DEFAULT 0,
    `needMoneyE` int(10) unsigned NOT NULL DEFAULT 0,
    `attrValueA` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValueB` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValueC` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValueD` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValueE` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValueF` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    `attrValueG` float(10, 2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`partId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
