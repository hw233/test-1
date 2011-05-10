ALTER TABLE `dungeon_player`  ADD COLUMN `difficulty` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `playerId`;
