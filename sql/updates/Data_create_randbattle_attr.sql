CREATE TABLE IF NOT EXISTS `randbattle_attr` (
    `lev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `id` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `value` float(5,2) NOT NULL DEFAULT '0.00',
    `next` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`lev`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
