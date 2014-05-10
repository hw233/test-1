DROP TABLE IF EXISTS `lingshi`;
CREATE TABLE `lingshi` (
    `id` int(10) unsigned NOT NULL DEFAULT 0,
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `isBreak` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `useItem` int(10) unsigned NOT NULL DEFAULT 0,
    `useGold` int(10) unsigned NOT NULL DEFAULT 0,
    `attack` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `magatk` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `defend` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `magdef` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `hp` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `toughlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `action` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `hitrlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `evdlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `crilvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `criticaldmg` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `pirlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `counterlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `mreslvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`id`, `level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `lingshi_upgrade`;
CREATE TABLE `lingshi_upgrade` (
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `consume` int(10) unsigned NOT NULL DEFAULT 0,
    `exp` bigint(20) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

