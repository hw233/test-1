CREATE TABLE IF NOT EXISTS `card` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `id` int(10) unsigned NOT NULL DEFAULT '0',
    `cid` int(10) unsigned NOT NULL DEFAULT '0',
    `level` tinyint(3) NOT NULL DEFAULT '0',
    `exp` int(10) NOT NULL DEFAULT '0',
    `pos` tinyint(3) NOT NULL DEFAULT '0',
    unique KEY id_playerid(`playerid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `cardsuit` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `id` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `suit_mark` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `active` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `spe_mark` int(10) unsigned NOT NULL DEFAULT '0',
    `collect_degree` tinyint(3) unsigned NOT NULL DEFAULT '0',
    unique KEY id_playerid(`playerid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

