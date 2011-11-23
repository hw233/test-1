
alter table `equipment` add column `tRank` tinyint(3) unsigned NOT NULL DEFAULT '0' after `enchant`;
alter table `equipment` add column `maxTRank` tinyint(3) unsigned NOT NULL DEFAULT '0' after `tRank`;
alter table `equipment` add column `trumpExp` int(10) unsigned NOT NULL DEFAULT '0' after `maxTRank`;

