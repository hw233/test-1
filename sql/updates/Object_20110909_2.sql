
DROP TABLE IF EXISTS `recharge`;
CREATE TABLE `recharge` (
      `no` char(255) NOT NULL DEFAULT '',
      `playerId` bigint(20) unsigned NOT NULL,
      `id` smallint(4) NOT NULL DEFAULT '29999',
      `num` int(10) unsigned NOT NULL,
      `status` tinyint(3) unsigned NOT NULL,
      PRIMARY KEY (`no`),
      KEY noplayer (`no`, `playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

