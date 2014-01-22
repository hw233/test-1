DROP TABLE IF EXISTS `coupleinfo`;
CREATE TABLE `coupleinfo` (
    `level` tinyint(3) unsigned NOT NULL,
    `levelExp` int(10) NOT NULL ,
    `hp` int(10) unsigned NOT NULL,
    `attak` int(10) unsigned  NOT NULL ,
    `magic_attak` int(10) unsigned NOT NULL,
    `df_critical` float(5,2) unsigned NOT NULL DEFAULT '0.00',
    `action` int(10) unsigned NOT NULL,
    PRIMARY KEY (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
