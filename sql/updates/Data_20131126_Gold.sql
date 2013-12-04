CREATE TABLE `acupragold` (
  `id` tinyint(3) unsigned NOT NULL,
  `lvl` tinyint(3) unsigned NOT NULL,
  `usereal` int(10) unsigned NOT NULL,
  `soulmax` int(10) unsigned NOT NULL,
  `attrnum` tinyint(3) unsigned NOT NULL,
  `attrvalue` float(10,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`, `lvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
