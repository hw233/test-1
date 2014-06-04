CREATE TABLE IF NOT EXISTS `clanbigboss` (
   `clanid` int(10) unsigned NOT NULL DEFAULT '0',
   `status`  tinyint(3) unsigned NOT NULL DEFAULT '0',
   `app_time` int(10) unsigned NOT NULL DEFAULT '0',
   `last` int(10) unsigned NOT NULL,
   `hp` int(10) unsigned NOT NULL,
   `atk` int(10) NOT NULL,
   `matk` int(10) NOT NULL,
   PRIMARY KEY (`clanid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;
