ALTER TABLE `clan_player` DROP COLUMN `taelCount`, DROP COLUMN `taelTime`;
INSERT INTO `clan_skill` SELECT `id`, 2, 0, 0 FROM `clan`;