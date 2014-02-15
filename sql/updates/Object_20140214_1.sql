
CREATE TABLE `mobao` (
   `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
   `buyNum`  int(10) unsigned NOT NULL DEFAULT '0',
   `status` int(10) unsigned NOT NULL DEFAULT '0',
   `item` varchar(1024) NOT NULL DEFAULT '',
   `itemACnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `itemBCnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `itemCCnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `itemDCnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `itemECnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `itemFCnt` tinyint(3) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

