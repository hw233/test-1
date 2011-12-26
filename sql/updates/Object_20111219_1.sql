
CREATE TABLE IF NOT EXISTS `clan_item`(
    `clanid` int(10) unsigned NOT NULL,
    `playerid` bigint(20) unsigned NOT NULL,
    `itemid` int(10) unsigned NOT NULL,
    `itemnum` int(10) unsigned DEFAULT 0,
    PRIMARY KEY(`clanid`, `playerid`, `itemid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `clan_item_history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `clanid` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `time` int(10) unsigned NOT NULL,
  `playerid` bigint(20) unsigned NOT NULL,
  `itemstr` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


