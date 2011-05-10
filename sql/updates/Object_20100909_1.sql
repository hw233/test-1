ALTER TABLE `day_task_instance`  ADD COLUMN `flushCount` INT(10) UNSIGNED NOT NULL COMMENT '当前颜色刷新次数' AFTER `currDay`,  DROP COLUMN `consumeGold`;
ALTER TABLE `player`  DROP COLUMN `donation`;
