ALTER TABLE `player`  ADD COLUMN `lastExp` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `totalRecharge`,  ADD COLUMN `lastResource` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER `lastExp`;
ALTER TABLE `dungeon_player`  CHANGE COLUMN `firstPass` `firstPass` VARCHAR(64) NOT NULL DEFAULT '0,0' AFTER `totalCount`;
