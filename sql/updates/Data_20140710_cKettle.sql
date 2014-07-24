

DROP TABLE IF EXISTS `kettleNpc`;
CREATE TABLE `kettleNpc` (
    `monsterId` int(10) unsigned NOT NULL DEFAULT 0,
    `attack` float(10,2) unsigned NOT NULL DEFAULT 0,
    `magatk` float(10,2) unsigned NOT NULL DEFAULT 0,
    `hp` float(10,2) unsigned NOT NULL DEFAULT 0,
    `action` float(10,2) unsigned NOT NULL DEFAULT 0,
    `exp` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`monsterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
