ALTER IGNORE TABLE `fighter_buff`  ADD COLUMN `playerId` BIGINT(20) UNSIGNED NOT NULL FIRST,  ADD INDEX `playerId` (`playerId`),  ADD UNIQUE INDEX `playerId_id` (`playerId`, `id`);
