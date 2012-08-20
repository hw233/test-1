
CREATE TABLE IF NOT EXISTS `second_soul` (
  `fighterId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `playerId` bigint(20) unsigned NOT NULL,
  `cls` tinyint(3) unsigned NOT NULL,
  `practiceLevel` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `stateLevel` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `stateExp` int(10) unsigned NOT NULL DEFAULT 0,
  `skills` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY(`fighterId`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


