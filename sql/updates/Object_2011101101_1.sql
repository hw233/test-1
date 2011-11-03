
DROP TABLE IF EXISTS `var`;
CREATE TABLE `var` (
      `playerId` bigint(20) unsigned NOT NULL,
      `id` smallint(5) NOT NULL,
      `data` int(10) unsigned NOT NULL,
      `over` int(10) unsigned NOT NULL,
      PRIMARY KEY(`playerId`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

