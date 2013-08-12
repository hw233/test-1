CREATE TABLE IF NOT EXISTS `tempItem` (
    `ownerId` bigint(20) unsigned NOT NULL,
    `id` int(10) unsigned NOT NULL DEFAULT '0',
    `itemNum` int(10) unsigned NOT NULL DEFAULT '0',
    `bind` smallint(5) unsigned NOT NULL DEFAULT '0',
    `sellTime` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`ownerId`, `id`, `bind`),
    KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
