ALTER TABLE `task_instance`  ADD INDEX `submit` (`submit`), DROP COLUMN `submitCount`;
DELETE FROM `friendliness` WHERE `fighterId` < 7;
