

DROP TABLE IF EXISTS `xinmo`;
CREATE TABLE `xinmo` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `limitLev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `consume` int(10) unsigned NOT NULL DEFAULT 0,
    `maxVal` int(10) unsigned NOT NULL DEFAULT 0,
    `attack` int(10) unsigned NOT NULL DEFAULT 0,
    `hp` int(10) unsigned NOT NULL DEFAULT 0,
    `action` int(10) unsigned NOT NULL DEFAULT 0,
    `cridec` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `skilllev` varchar(255) NOT NULL DEFAULT '',
    `payBack` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
