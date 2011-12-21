
CREATE TABLE IF NOT EXISTS `player_honeyfall` (
  `playerId` bigint(20) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `value` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`, `type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


