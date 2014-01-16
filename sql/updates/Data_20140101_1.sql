DROP TABLE IF EXISTS `petteamcopy`;
CREATE TABLE `petteamcopy` (
  `copyIdx` tinyint(3) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `npcgroupId` int(10) unsigned NOT NULL,
  `quality` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`npcgroupId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

