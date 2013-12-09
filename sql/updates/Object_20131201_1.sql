CREATE TABLE IF NOT EXISTS `arena_serverWar` (
  `playerId` bigint(20) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `pos` int(10) unsigned NOT NULL DEFAULT '0',
  `battlePoint` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`playerId`, `type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `arena_serverWar_bet` (
  `id` bigint(20) unsigned NOT NULL,
  `round` tinyint(3) unsigned NOT NULL,
  `state` tinyint(3) unsigned NOT NULL,
  `group` tinyint(3) unsigned NOT NULL,
  `recieved` tinyint(3) unsigned NOT NULL,
  `pos` smallint(4) unsigned NOT NULL,
  `tael` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`,`state`,`round`,`group`,`pos`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

