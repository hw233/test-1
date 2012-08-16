
CREATE TABLE `shuoshuo` (
      `playerId` bigint(20) unsigned NOT NULL,
      `updateTime` int(10) NOT NULL,
      `shuoshuo` varchar(255) NOT NULL DEFAULT '',
      PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

