CREATE TABLE `teammember` (
   `teamId` int(10) unsigned NOT NULL DEFAULT '0',
   `member1` bigint(20) unsigned NOT NULL DEFAULT '0',
   `member2` bigint(20) unsigned NOT NULL DEFAULT '0',
   `member3` bigint(20) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`teamId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

