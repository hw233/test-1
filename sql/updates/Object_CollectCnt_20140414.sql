CREATE TABLE `collect_cnt` (
   `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
   `lev` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `bluecnt` int(10) unsigned NOT NULL DEFAULT '0',
   `purlecnt` int(10) unsigned NOT NULL DEFAULT '0',
   `orangecnt` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`playerId`,`lev`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;


