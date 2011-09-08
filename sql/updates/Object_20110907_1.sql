alter table `player` add column `qqvipl` tinyint(3) unsigned NOT NULL DEFAULT '0' after `archievement`;
alter table `player` add column `qqvipyear` tinyint(3) unsigned NOT NULL DEFAULT '0' after `archievement`;


alter table `athletics_rank` modify column `extrachallenge` int(10) unsigned NOT NULL DEFAULT '0';

alter table `practice_data` add column `hookadd` tinyint(2) unsigned NOT NULL DEFAULT '0' after `winnerid`;
alter table `practice_data` add column `nexthareset` int(10) unsigned NOT NULL DEFAULT '0' after `hookadd`;
