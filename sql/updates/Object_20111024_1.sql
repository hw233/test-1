
alter table `player` add column `prestige` int(10) unsigned NOT NULL DEFAULT '0' after `coin`;
alter table `athletics_rank` add column `tael` int(10) unsigned NOT NULL DEFAULT '0' after `prestige`;

update `player` a, `athletics_rank` b set a.prestige=b.prestige where a.id=b.ranker;
update `athletics_rank` set prestige=0;


