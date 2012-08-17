
CREATE TABLE IF NOT EXISTS `new_relation` (
    `playerId` bigint(20) unsigned NOT NULL,
    `mood` smallint(3) unsigned NOT NULL DEFAULT 0,
    `sign` varchar(255) NOT NULL,
    PRIMARY KEY(`playerId`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
