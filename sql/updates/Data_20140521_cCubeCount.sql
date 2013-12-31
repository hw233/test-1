CREATE TABLE `cubeCount` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `index` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `cnt` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`,`index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
