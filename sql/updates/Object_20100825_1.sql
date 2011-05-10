ALTER TABLE `player`  ADD INDEX `mainFighter` (`mainFighter`);
ALTER TABLE `dungeon_player`  ADD COLUMN `counterEnd` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `firstPass`;
