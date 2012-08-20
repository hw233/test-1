
CREATE TABLE IF NOT EXISTS `auto_frontmap` (
    `playerId` bigint(20) unsigned NOT NULL,
    `id` tinyint(3) unsigned NOT NULL,
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

alter table `athletics_rank` add column `pageNum` tinyint(1) unsigned NOT NULL DEFAULT '0' after `extrachallenge`;

