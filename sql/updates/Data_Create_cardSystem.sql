DROP TABLE IF EXISTS `cardInfo`;
CREATE TABLE `cardInfo` (
    `id` int(10) unsigned NOT NULL DEFAULT 0,
    `type` tinyint(3) NOT NULL DEFAULT 0,
    `color` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `lvLimit` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `skillId` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `cardupgrade`;
CREATE TABLE `cardupgrade` (
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `gexp` int(10) NOT NULL DEFAULT 0,
    `bexp` int(10) NOT NULL DEFAULT 0,
    `pexp` int(10) NOT NULL DEFAULT 0,
    `yexp` int(10) NOT NULL DEFAULT 0,
    `hgexp` int(10) NOT NULL DEFAULT 0,
    `hbexp` int(10) NOT NULL DEFAULT 0,
    `hpexp` int(10) NOT NULL DEFAULT 0,
    `hyexp` int(10) NOT NULL DEFAULT 0,
    `skillLevel` tinyint(3) NOT NULL DEFAULT 0,
    `attrIndex` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
