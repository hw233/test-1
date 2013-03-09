DROP TABLE IF EXISTS `pet_pinjie`;
CREATE TABLE `pet_pinjie` (
    `id` smallint(6) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `consume` int(10) unsigned NOT NULL DEFAULT 0,
    `prob` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `pet_gengu`;
CREATE TABLE `pet_gengu` (
    `id` smallint(6) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `limit` smallint(6) unsigned NOT NULL DEFAULT 0,
    `baseProb` int(10) unsigned NOT NULL DEFAULT 0,
    `failBack` smallint(6) unsigned NOT NULL DEFAULT 0,
    `consume1` int(10) unsigned NOT NULL DEFAULT 0,
    `consume2` int(10) unsigned NOT NULL DEFAULT 0,
    `growRate` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `pet_pressure`;
CREATE TABLE `pet_pressure` (
    `id` int(10) unsigned NOT NULL DEFAULT 0,
    `color` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `lingya` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

