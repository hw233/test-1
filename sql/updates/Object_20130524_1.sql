ALTER TABLE `fairyPet` ADD COLUMN `evolve` smallint(6) unsigned NOT NULL DEFAULT '0' after `dazhou`;
ALTER TABLE `fairyPet` ADD COLUMN `equip1` int(10) unsigned NOT NULL DEFAULT '0' after `evolve`;
ALTER TABLE `fairyPet` ADD COLUMN `equip2` int(10) unsigned NOT NULL DEFAULT '0' after `equip1`;
ALTER TABLE `fairyPet` ADD COLUMN `equip3` int(10) unsigned NOT NULL DEFAULT '0' after `equip2`;

CREATE TABLE IF NOT EXISTS `petEquipattr` (
    `id` int(10) unsigned NOT NULL,
    `level` tinyint(3) unsigned NOT NULL,
    `exp` int(10) unsigned NOT NULL,
    `skillId` smallint(10) unsigned NOT NULL,
    `socket1` smallint(10) unsigned NOT NULL,
    `socket2` smallint(10) unsigned NOT NULL,
    `socket3` smallint(10) unsigned NOT NULL,
    `socket4` smallint(10) unsigned NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
