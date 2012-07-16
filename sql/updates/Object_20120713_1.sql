
CREATE TABLE IF NOT EXISTS `skill_strengthen` (
    `id` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `skillid` smallint(5) unsigned NOT NULL,
    `father` smallint(5) unsigned NOT NULL,
    `maxVal` int(10) unsigned NOT NULL,
    `curVal` int(10) unsigned NOT NULL,
    `lvl` tinyint(1) unsigned NOT NULL,
    UNIQUE KEY `id_playerId_skill` (`id`,`playerId`, `skillid`),
    KEY `playerId` (`playerId`),
    KEY `id` (`id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
