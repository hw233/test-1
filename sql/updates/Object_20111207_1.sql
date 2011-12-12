
CREATE TABLE IF NOT EXISTS `teamcopy_player_award` (
  `playerId` bigint(20) unsigned NOT NULL,
  `rollId` tinyint(3) unsigned NOT NULL,
  `roll` tinyint(3) unsigned NOT NULL,
  `awardId` int(10) unsigned NOT NULL,
  `awardCnt` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



