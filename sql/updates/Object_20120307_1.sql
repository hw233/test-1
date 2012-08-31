CREATE TABLE `cfriend_awards` (
      `playerId` bigint(20) unsigned NOT NULL,
      `invitedId` bigint(20) unsigned NOT NULL DEFAULT 0,
      `awards` varchar(256) NOT NULL DEFAULT '',
      PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
