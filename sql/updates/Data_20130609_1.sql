DROP TABLE IF EXISTS `xingchen`;
CREATE TABLE `xingchen` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `limitLev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `consume` int(10) unsigned NOT NULL DEFAULT 0,
    `maxVal` int(10) unsigned NOT NULL DEFAULT 0,
    `rate1` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `rate2` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `rate3` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `payBack` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

