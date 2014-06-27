DROP TABLE IF EXISTS `geartree`;
CREATE TABLE `geartree` (
    `suiteId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `lastSuiteId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `partAId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `componentAId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `componentBId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `partBId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `componentCId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `componentDId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `partCId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `componentEId` smallint(5) unsigned NOT NULL DEFAULT 0,
    `componentFId` smallint(5) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`suiteId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
