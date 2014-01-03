CREATE TABLE IF NOT EXISTS `modify_mount` (
    `id` int(10) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `chips` varchar(255) NOT NULL,
    PRIMARY KEY (`id`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

