CREATE TABLE IF NOT EXISTS `player_named` (
    `serverNo` int(10) unsigned NOT NULL DEFAULT '0',
    `playerid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `name` varchar(255) NOT NULL,
    unique KEY `serverNum_name`(serverNo,`name`),
    KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


