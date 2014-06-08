DROP TABLE IF EXISTS `lingshi_level`;
CREATE TABLE `lingshi_level` (
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `consume` int(10) unsigned NOT NULL DEFAULT 0,
    `exp` bigint(20) unsigned NOT NULL DEFAULT 0,
    `isBreak` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `useItem` int(10) unsigned NOT NULL DEFAULT 0,
    `useGold` int(10) unsigned NOT NULL DEFAULT 0,
    `attack` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `magatk` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `hp` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `action` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

