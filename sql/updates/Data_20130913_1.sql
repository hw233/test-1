DROP TABLE IF EXISTS `pet_sanhun`;
CREATE TABLE `pet_sanhun` (
    `lvl` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `attr1` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr2` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr3` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr4` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr5` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr6` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr7` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr8` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attr9` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `money1` int(10) unsigned NOT NULL DEFAULT 0,
    `money2` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`lvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
