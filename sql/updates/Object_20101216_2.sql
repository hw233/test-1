ALTER TABLE `task_instance`  DROP COLUMN `id`;
ALTER IGNORE TABLE `day_task_instance`  DROP COLUMN `id`,  DROP PRIMARY KEY,  ADD PRIMARY KEY (`loopTask`, `ownerId`);