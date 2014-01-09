CREATE TABLE IF NOT EXISTS `reply_marriage` (
    `man_playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `woman_playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `jy_time` int(10) unsigned NOT NULL DEFAULT '0',
    `jh_time` int(10) unsigned NOT NULL DEFAULT '0',
    `wedding_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `wedding_buyer` tinyint(3) unsigned NOT NULL DEFAULT '0',
    unique KEY man_woman_playerid(`man_playerid`,`woman_playerid`),
    INDEX (`man_playerid`),
    INDEX (`woman_playerid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
