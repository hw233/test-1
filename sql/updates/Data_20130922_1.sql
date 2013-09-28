DROP TABLE IF EXISTS `zhenwei`;
CREATE TABLE `zhenwei` (
    `keyId` smallint(3) unsigned NOT NULL DEFAULT 0,
    `name` varchar(255) NOT NULL DEFAULT '',
    `collectType` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `collect1` int(10) unsigned NOT NULL DEFAULT 0,
    `collect2` int(10) unsigned NOT NULL DEFAULT 0,
    `collect3` int(10) unsigned NOT NULL DEFAULT 0,
    `collect4` int(10) unsigned NOT NULL DEFAULT 0,
    `collect5` int(10) unsigned NOT NULL DEFAULT 0,
    `collect6` int(10) unsigned NOT NULL DEFAULT 0,
    `collect7` int(10) unsigned NOT NULL DEFAULT 0,
    `collect8` int(10) unsigned NOT NULL DEFAULT 0,
    `award` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`keyId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
