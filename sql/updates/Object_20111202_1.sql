
CREATE TABLE IF NOT EXISTS `teamcopy_player` (
  `playerId` bigint(20) unsigned NOT NULL,
  `copyId` tinyint(3) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `pass` tinyint(3) unsigned NOT NULL,
  `passTimes` tinyint(3) unsigned NOT NULL,
  `vTime` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`,`copyId`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


