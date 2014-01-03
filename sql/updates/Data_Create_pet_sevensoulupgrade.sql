CREATE TABLE IF NOT EXISTS `sevensoulupgrade` (
  `pettype` tinyint(3) unsigned NOT NULL,
  `soulid` tinyint(3) unsigned NOT NULL,
  `skillstr` varchar(255) NOT NULL,
  `condionvalue` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`pettype`, `soulid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
