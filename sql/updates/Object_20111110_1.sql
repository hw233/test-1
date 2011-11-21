
--帮会排名战报名时间
ALTER TABLE `clan_player` ADD COLUMN `signupRankBattleTime` int(10) unsigned NOT NULL DEFAULT '0';

--帮会排名战积分
ALTER TABLE `clan` ADD COLUMN `battleScore` int(10) unsigned NOT NULL DEFAULT '0';

--帮会排名战战役
ALTER TABLE `clan_player` ADD COLUMN `rankBattleField` int(10) unsigned NOT NULL DEFAULT '0'; 

--帮会战上周名次
ALTER TABLE `clan` ADD COLUMN `battleRanking` int(10) unsigned NOT NULL DEFAULT '0';

