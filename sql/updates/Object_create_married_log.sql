CREATE TABLE IF NOT EXISTS `married_log` (
    `jh_time` int(10) unsigned NOT NULL DEFAULT '0',
    `man_playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `woman_playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `prouncement` varchar(255) NOT NULL,
    `lover_item` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `marriage_time` int(10) unsigned NOT NULL DEFAULT '0',
    `replymarriage_time` int(10) unsigned NOT NULL DEFAULT '0',
    `jy_time` int(10) unsigned NOT NULL DEFAULT '0',
    `wedding_type` tinyint(3) unsigned NOT NULL DEFAULT  '0',
    PRIMARY KEY(`jh_time`),
    unique KEY man_woman_playerid(`man_playerid`,`woman_playerid`),
    INDEX (`man_playerid`),
    INDEX (`woman_playerid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
