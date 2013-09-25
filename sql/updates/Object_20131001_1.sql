
CREATE TABLE IF NOT EXISTS `arena_team` (
    `id` bigint(20) unsigned NOT NULL,
    `name` varchar(255) NOT NULL DEFAULT '',
    `leader` bigint(20) unsigned NOT NULL,
    `score` int(10) unsigned NOT NULL,
    `members` varchar(255) NOT NULL DEFAULT '',
    `stamps` varchar(255) NOT NULL DEFAULT '',
    `memScore` varchar(255) NOT NULL DEFAULT '',
    `maxRank` smallint(6) NOT NULL DEFAULT '0',
    `inspireLvl` tinyint(3) NOT NULL DEFAULT '0',
    `overTime` int(10) NOT NULL DEFAULT '0',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `arena_team_skill` (
  `teamId` bigint(20) unsigned NOT NULL,
  `skillId` tinyint(3) unsigned NOT NULL,
  `level` tinyint(3) unsigned NOT NULL,
  `extra` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`teamId`,`skillId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `arena_team_bet` (
  `id` bigint(20) unsigned NOT NULL,
  `round` tinyint(3) unsigned NOT NULL,
  `state` tinyint(3) unsigned NOT NULL,
  `group` tinyint(3) unsigned NOT NULL,
  `recieved` tinyint(3) unsigned NOT NULL,
  `pos` smallint(4) unsigned NOT NULL,
  `tael` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`,`state`,`round`,`group`,`pos`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

