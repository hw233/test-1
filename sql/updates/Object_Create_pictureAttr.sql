CREATE TABLE IF NOT EXISTS `pictureAttr` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `floor` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `cubeHave` varchar(256) NOT NULL DEFAULT '',
    `cubeCover` varchar(1024) NOT NULL DEFAULT '',
    unique KEY id_playerid(`playerid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
