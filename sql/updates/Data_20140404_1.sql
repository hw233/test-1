DROP TABLE IF EXISTS `zhenyuan_extraAttr`;
CREATE TABLE `zhenyuan_extraAttr` (
    `id` int(10) unsigned NOT NULL,
    `lvLimit` tinyint(3) unsigned NOT NULL,
    `type1` tinyint(3) unsigned NOT NULL,
    `type2` tinyint(3) unsigned NOT NULL,
    `maxVal` int(10) unsigned NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

