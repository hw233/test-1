DROP TABLE IF EXISTS `picInfo`;
CREATE TABLE `picInfo` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `index` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `cost` int(10) unsigned NOT NULL DEFAULT 0,
    `buff` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`,`index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
