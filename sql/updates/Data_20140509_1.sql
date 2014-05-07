DROP TABLE IF EXISTS `erlking`;
CREATE TABLE `erlking` (
    `copyId` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `conditionA` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `conditionB` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `npcgroup` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`copyId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

