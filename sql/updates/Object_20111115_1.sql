
DROP TABLE IF EXISTS `auto_frontmap`;
CREATE TABLE `auto_frontmap` (
    `playerId` bigint(20) unsigned NOT NULL,
    `id` tinyint(3) unsigned NOT NULL,
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

