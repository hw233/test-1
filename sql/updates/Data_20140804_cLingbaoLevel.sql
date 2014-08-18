DROP TABLE IF EXISTS `lingbaoLevel`;
CREATE TABLE `lingbaoLevel` (
    `id` int(10) unsigned NOT NULL DEFAULT 0,
    `itemNum` int(10) unsigned NOT NULL DEFAULT 0,
    `itemCount` int(10) unsigned NOT NULL DEFAULT 0,
    `percent` float(10,2) unsigned NOT NULL DEFAULT 0,
    `probability` int(10) unsigned NOT NULL DEFAULT 0,
    `HoneyFall` int(10) unsigned NOT NULL DEFAULT 0,
    `HoneyFallP` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
