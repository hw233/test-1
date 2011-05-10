UPDATE `clan_skill` SET `extra` = 0 WHERE `extra` < 0;
ALTER TABLE `clan_skill` CHANGE COLUMN `extra` `extra` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `clan` ADD COLUMN `grabAchieve` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0' AFTER `proffer`;
UPDATE `clan` SET `battleTime` = 25, `nextBattleTime` = 25;