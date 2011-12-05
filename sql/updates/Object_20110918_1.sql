update `tripod` set `regen` = 1;
alter table `tripod` add column `itemId` int(10) unsigned NOT NULL  DEFAULT '0' after `regen`;
alter table `tripod` add column `num` tinyint(5) unsigned NOT NULL DEFAULT '0' after `itemId`;
