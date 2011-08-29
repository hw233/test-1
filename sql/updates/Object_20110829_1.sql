
alter table `athletics_rank` modify column `winStreak` smallint(3) unsigned NOT NULL DEFAULT '0';
alter table `athletics_rank` modify column `bewinstreak` smallint(3) unsigned NOT NULL DEFAULT '0';
alter table `athletics_rank` modify column `failstreak` smallint(3) unsigned NOT NULL DEFAULT '0';
alter table `athletics_rank` modify column `befailstreak` smallint(3) unsigned NOT NULL DEFAULT '0';
alter table `athletics_rank` modify column `first4rank` int(10) unsigned NOT NULL DEFAULT '0';
alter table `athletics_rank` modify column `extrachallenge` int(10) NOT NULL DEFAULT '0';


