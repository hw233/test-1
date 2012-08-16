CREATE TABLE IF NOT EXISTS  `heromemo` (
      `playerId` bigint(20) unsigned NOT NULL,
      `awards` varchar(255) NOT NULL DEFAULT '',
      `memos` varchar(512) NOT NULL DEFAULT '',
      PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

