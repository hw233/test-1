ALTER TABLE `player`  ADD COLUMN `totalRecharge` INT(32) UNSIGNED NOT NULL DEFAULT '0' AFTER `lineup`, ADD COLUMN `nextReward` INT(32) UNSIGNED NOT NULL DEFAULT '0' AFTER `totalRecharge`;
UPDATE `player` SET `status` = `status` & 0xFFFFFF7F;
UPDATE `player` SET `status` = `status` & 0xFFF7FFFF, `totalRecharge` = 1 WHERE (`status` & 0x80000) > 0;
