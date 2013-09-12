 CREATE TABLE IF NOT EXISTS `player_presentbox` (
    `id` bigint(20) unsigned NOT NULL DEFAULT '0',
    `awardId`   int(10) unsigned NOT NULL DEFAULT '0',
    `playerId2` bigint(20) unsigned NOT NULL DEFAULT '0',
    `sendtime`  int(10) unsigned NOT NULL DEFAULT '0',
    `get` tinyint(1) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`id`,`playerId2`, `sendtime`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

