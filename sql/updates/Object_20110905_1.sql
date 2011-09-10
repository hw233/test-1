DROP TABLE IF EXISTS `autocopy`;
CREATE TABLE `autocopy` (
    `playerId` bigint(20) unsigned NOT NULL,
    `id` tinyint(3) unsigned NOT NULL,
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
