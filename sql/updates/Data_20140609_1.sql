DROP TABLE IF EXISTS `clan_contribution_shop`;
CREATE TABLE `clan_contribution_shop` (
    `id` smallint(6) unsigned NOT NULL,
    `itemid` smallint(6) unsigned NOT NULL,
    `name` varchar(255) NOT NULL DEFAULT '',
    `price` int(10) unsigned NOT NULL DEFAULT 0,
    `prob` int(10) unsigned NOT NULL DEFAULT 0,
    `color` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `lvl` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

