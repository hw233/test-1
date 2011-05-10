ALTER TABLE `clan_player` DROP COLUMN `enterClanBattleTime`, CHANGE COLUMN `achieveTime` `thisDay` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `enterCount`;
