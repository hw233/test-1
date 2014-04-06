CREATE TABLE IF NOT EXISTS `player_xjfrontmap` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` tinyint(3) unsigned NOT NULL,
  `spot` tinyint(3) unsigned NOT NULL,
  `count` tinyint(3) unsigned NOT NULL,
  `status` tinyint(3) unsigned NOT NULL,
  `lootlvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `player_id_spot` (`playerId`,`id`, `spot`),
  KEY `playerIdId` (`playerId`, `id`),
  KEY `playerId` (`playerId`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `auto_xjfrontmap` (
  `playerId` bigint(20) unsigned NOT NULL,
  `id` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `zhenyuanAttr` (
   `id` int(10) unsigned NOT NULL DEFAULT '0',
   `itemId`  int(10) unsigned NOT NULL DEFAULT '0',
   `zycolor` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `types` varchar(255) NOT NULL DEFAULT '',
   `values` varchar(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

ALTER TABLE player ADD column `xjfrontFreeCnt` tinyint(3) unsigned NOT NULL DEFAULT 0;
ALTER TABLE player ADD column `xjfrontGoldCnt` tinyint(3) unsigned NOT NULL DEFAULT 0;
ALTER TABLE player ADD column `xjfrontUpdate` int(10) unsigned NOT NULL DEFAULT 0;
ALTER TABLE player ADD column `zhenyuans` varchar(1024) NOT NULL DEFAULT '' after `formations`;

