ALTER TABLE `fighter`  ADD INDEX `playerId` (`playerId`);
ALTER TABLE `fighter_buff`  ADD INDEX `id` (`id`);
ALTER TABLE `task_instance`  ADD INDEX `ownerId` (`ownerId`);
ALTER TABLE `item`  ADD INDEX `id` (`id`),  ADD INDEX `ownerId` (`ownerId`);
ALTER TABLE `day_task_instance`  ADD INDEX `ownerId` (`ownerId`);
ALTER TABLE `clan_player`  ADD INDEX `id` (`id`),  ADD INDEX `playerId` (`playerId`);
