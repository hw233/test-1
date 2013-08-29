CREATE TABLE IF NOT EXISTS `pray_relation` (                       
    `id` bigint(20) unsigned NOT NULL,
    `friendId` bigint(20) unsigned NOT NULL,
    `pray` tinyint(1) unsigned NOT NULL DEFAULT '0',
    `time` int(10) unsigned NOT NULL,
    UNIQUE KEY `id_friendId` (`id`,`friendId`),
    KEY `id` (`id`)
  ) 
ENGINE=InnoDB DEFAULT CHARSET=utf8;



