alter table `clan`  add column `qqOpenid` varchar(36) NULL after `battleRanking`;
alter table `clan_player` add column `inQQGroup` tinyint(1) DEFAULT '0' NULL after `rankBattleField`;
