DROP TABLE IF EXISTS `shake_moneybag_log`;
CREATE TABLE IF NOT EXISTS `shake_moneybag_log` (
     `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
     `data` int(10) unsigned NOT NULL,
     `count` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
