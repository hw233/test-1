CREATE TABLE `duobaolog` (
  `clanId` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `score` int(10) unsigned NOT NULL DEFAULT '0',
  `itemId` int(10) unsigned NOT NULL DEFAULT '0',
  `cnt` tinyint(10) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;
