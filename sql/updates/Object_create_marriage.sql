CREATE TABLE IF NOT EXISTS `marriage` (
    `playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `marriage_time` int(10) unsigned NOT NULL DEFAULT '0',
    `prouncement` varchar(255) NOT NULL,
    `lover_item` varchar(255) NOT NULL,
    `status` bigint(20) NOT NULL,
    PRIMARY KEY(`playerid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
