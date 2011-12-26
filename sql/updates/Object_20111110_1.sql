
ALTER TABLE `clan` ADD COLUMN `dailyBattleScore` int(10) unsigned NOT NULL DEFAULT '0';

ALTER TABLE `clan_player` ADD COLUMN `signupRankBattleTime` int(10) unsigned NOT NULL DEFAULT '0';

ALTER TABLE `clan` ADD COLUMN `battleScore` int(10) unsigned NOT NULL DEFAULT '0';

ALTER TABLE `clan_player` ADD COLUMN `rankBattleField` int(10) unsigned NOT NULL DEFAULT '0'; 

ALTER TABLE `clan` ADD COLUMN `battleRanking` int(10) unsigned NOT NULL DEFAULT '0';



