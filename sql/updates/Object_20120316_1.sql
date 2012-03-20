CREATE TABLE `enchlog` (
      `playerId` bigint(20) unsigned NOT NULL,
      `type` tinyint(3) unsigned NOT NULL,
      `enchant` tinyint(3) unsigned NOT NULL,
      PRIMARY KEY (`playerId`,`type`,`enchant`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
