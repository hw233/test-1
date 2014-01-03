DROP TABLE IF EXISTS `ride`;
CREATE TABLE `ride` (
    `id` tinyint(3) unsigned NOT NULL,
    `name` varchar(255) NOT NULL DEFAULT '',
    `itemId` int(10) unsigned NOT NULL,
    `chips` varchar(255) NOT NULL DEFAULT '',
    `propId` int(10) unsigned NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;

DROP TABLE IF EXISTS `ride_upgrade`;
CREATE TABLE `ride_upgrade` (
    `level` tinyint(3) unsigned NOT NULL,
    `name` varchar(255) NOT NULL DEFAULT '',
    `lvLimit` int(10) unsigned NOT NULL,
    `singleCost` int(10) unsigned NOT NULL,
    `lvExp` int(10) unsigned NOT NULL,
    `rate` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
