CREATE TABLE IF NOT EXISTS `questionpaper` (
    `playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `begtime` int(10) unsigned NOT NULL DEFAULT '0',
    `cur_idx` tinyint(3) unsigned NOT NULL DEFAULT  '0',
    `answer` varchar(255) NOT NULL DEFAULT '',
    `award` varchar(255) NOT NULL DEFAULT '',
    `totaltime` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`playerid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
