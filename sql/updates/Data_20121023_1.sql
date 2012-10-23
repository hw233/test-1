DELETE FROM `clan_copy_monster_template`;
ALTER TABLE `clan_copy_monster_template` ADD COLUMN `npcIndex` int (10) unsigned NOT NULL PRIMARY KEY FIRST;
