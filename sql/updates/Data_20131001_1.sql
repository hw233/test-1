
DROP TABLE IF EXISTS `team_arena_skill`;
CREATE TABLE `team_arena_skill` (
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `needs` int(10) unsigned NOT NULL DEFAULT 0,
    `teamLev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `attack` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `hp` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `defend` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `magdef` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `action` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `magresP` float(5,4) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`id`, `level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `team_arena_inspire`;
CREATE TABLE `team_arena_inspire` (
    `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `consume` int(10) unsigned NOT NULL DEFAULT 0,
    `rate` int(10) unsigned NOT NULL DEFAULT 0,
    `attackP` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `defendP` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `hpP` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `actionP` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

