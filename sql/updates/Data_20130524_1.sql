DROP TABLE IF EXISTS `pet_equipExp`;
CREATE TABLE `pet_equipExp` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `green` int(10) unsigned NOT NULL DEFAULT 0,
    `blue` int(10) unsigned NOT NULL DEFAULT 0,
    `purple` int(10) unsigned NOT NULL DEFAULT 0,
    `yellow` int(10) unsigned NOT NULL DEFAULT 0,
    `skillLev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `pet_eqAttreffect`;
CREATE TABLE `pet_eqAttreffect` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `hp` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `atk` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `magatk` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `def` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `magdef` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `critical` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `pierce` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `hitrate` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `evade` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `counter` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `tough` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

