alter table `practice_place` add column `serverId` smallint(5) unsigned NOT NULL NULL DEFAULT '0';
alter table `practice_place` drop primary key;
alter table `practice_place` add primary key (id, serverId);
alter table `clan` add column `serverId` smallint(5) unsigned NOT NULL DEFAULT '0';
