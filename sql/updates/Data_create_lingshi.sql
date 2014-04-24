DROP TABLE IF EXISTS `lingshi`;
CREATE TABLE `lingshi` (
    `id` int(10) unsigned NOT NULL DEFAULT 0,
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `isUp` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `useItem` int(10) unsigned NOT NULL DEFAULT 0,
    `useGold` int(10) unsigned NOT NULL DEFAULT 0,
    `attack` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `magatk` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `defend` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `magdef` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `hp` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `toughlvl` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `action` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `hitrlvl` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `evdlvl` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `crilvl` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `criticaldmg` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `pirlvl` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `counterlvl` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `mreslvl` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`id`, `level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `lingshi_upgrade`;
CREATE TABLE `lingshi_upgrade` (
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `rate` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `exp` bigint(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

