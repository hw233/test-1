CREATE TABLE IF NOT EXISTS `SpecialAward` (
  `rank` tinyint(2) unsigned NOT NULL,
  `type` tinyint(1) unsigned NOT NULL,
  `playerId` bigint(20) unsigned NOT NULL,
  `awardtime`int(10) unsigned NOT NULL
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;