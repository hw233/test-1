
DROP TABLE IF EXISTS `cubeAttr`;
CREATE TABLE `cubeAttr` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `hp` int(10) unsigned NOT NULL DEFAULT 0,
    `attack` float(6,2) NOT NULL DEFAULT '0.00',
    `action` float(6,2) NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
