CREATE TABLE IF NOT EXISTS `clan_statue` (
    `clanId` bigint(20) unsigned NOT NULL,
    `level` int(10) unsigned NOT NULL,
    `exp` bigint(20) unsigned NOT NULL,
    `expUpdateTime` bigint(20) unsigned NOT NULL,
    PRIMARY KEY(`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `clan_copy` (
    `clanId` bigint(20) unsigned NOT NULL,
    `level` int(10) unsigned NOT NULL,
    `levelUpdateTime` bigint(20) unsigned NOT NULL,
    `maxCopyLevel` int(10) unsigned NOT NULL DEFAULT '0',
    `maxCopyTime` bigint(20) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `clan_copy_log` (
    `clanId` bigint(20) unsigned NOT NULL,
    `logTime` bigint(20) unsigned NOT NULL,
    `logType` smallint(6) NOT NULL,
    `playerName` varchar(255) NOT NULL,
    `logVal` bigint(20) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

