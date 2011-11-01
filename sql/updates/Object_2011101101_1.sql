
DROP TABLE IF EXISTS `var`;
CREATE TABLE `var` (
      `id` bigint(20) unsigned NOT NULL,
      `buffId` smallint(5) NOT NULL,
      `data` int(10) unsigned NOT NULL,
      `over` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
      UNIQUE KEY `id_var` (`id`,`buffId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

