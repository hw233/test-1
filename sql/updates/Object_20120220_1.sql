
CREATE TABLE IF NOT EXISTS  `autoboss` (
      `playerId` bigint(20) unsigned NOT NULL,
      `whichday` int(10) unsigned NOT NULL,
      `cfg` varchar(255) NOT NULL DEFAULT '',
      PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

