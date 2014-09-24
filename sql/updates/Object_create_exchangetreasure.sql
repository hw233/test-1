CREATE TABLE IF NOT EXISTS `exchange_treasure_log` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `data` int(10) unsigned NOT NULL DEFAULT '0',
    `itemid` int(10) unsigned NOT NULL DEFAULT '0',
    `count` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

