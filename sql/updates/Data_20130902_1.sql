DROP TABLE IF EXISTS `signet`;
CREATE TABLE `signet` (
    `keyinId` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `lvl` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `quality` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `attrType` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `attrValue` int(10) unsigned NOT NULL DEFAULT 0,
    `materialA` int(10) unsigned NOT NULL DEFAULT 0,
    `materialB` int(10) unsigned NOT NULL DEFAULT 0,
    `maxValue` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`keyinId`, `lvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
