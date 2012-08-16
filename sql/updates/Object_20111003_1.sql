alter table `player_frontmap` add column `lootlvl` tinyint(3) unsigned NOT NULL DEFAULT '0' after `status`;
alter table `player_copy` add column `lootlvl` tinyint(3) unsigned NOT NULL DEFAULT '0' after `spot`;
