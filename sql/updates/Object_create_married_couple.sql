CREATE TABLE IF NOT EXISTS `married_couple` (
    `jh_time` int(10) unsigned NOT NULL DEFAULT '0',
    `man_playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `woman_playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `lover_item` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `both_onlinetime` int(10) unsigned NOT NULL DEFAULT '0',
    `pet_name` varchar(255) NOT NULL,
    `pet_level` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `pet_levelExp` int(10) unsigned NOT NULL DEFAULT '0',
    `pet_friendliness` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`jh_time`),
    unique KEY man_woman_playerid(`man_playerid`,`woman_playerid`),
    INDEX (`man_playerid`),
    INDEX (`woman_playerid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
