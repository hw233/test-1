alter table `athletics_rank` add column `eChallengeTime` int(10) unsigned NOT NULL DEFAULT '0'after `pageNum`;
alter table `athletics_rank` add column `ePhysical` tinyint(3) unsigned NOT NULL DEFAULT '30' after `eChallengeTime`;

alter table `athletics_rank` add column `eSelectIndex` tinyint(3) unsigned NOT NULL DEFAULT '0' after `ePhysical`;

alter table `athletics_rank` add column `eCombine1` int(10) unsigned NOT NULL DEFAULT '0'after `eSelectIndex`;
alter table `athletics_rank` add column `eCombine2` int(10) unsigned NOT NULL DEFAULT '0'after `eCombine1`;
alter table `athletics_rank` add column `eCombine3` int(10) unsigned NOT NULL DEFAULT '0'after `eCombine2`;
alter table `athletics_rank` add column `eCombine4` int(10) unsigned NOT NULL DEFAULT '0'after `eCombine3`;
alter table `athletics_rank` add column `eCombine5` int(10) unsigned NOT NULL DEFAULT '0'after `eCombine4`;

alter table `athletics_rank` add column `eRival1` bigint(20) unsigned NOT NULL DEFAULT '0'after `eCombine5`;
alter table `athletics_rank` add column `eRival2` bigint(20) unsigned NOT NULL DEFAULT '0'after `eRival1`;
alter table `athletics_rank` add column `eRival3` bigint(20) unsigned NOT NULL DEFAULT '0'after `eRival2`;
alter table `athletics_rank` add column `eRival4` bigint(20) unsigned NOT NULL DEFAULT '0'after `eRival3`;
alter table `athletics_rank` add column `eRival5` bigint(20) unsigned NOT NULL DEFAULT '0'after `eRival4`;

alter table `athletics_rank` add column `eCanAttack1` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eRival5`;
alter table `athletics_rank` add column `eCanAttack2` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eCanAttack1`;
alter table `athletics_rank` add column `eCanAttack3` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eCanAttack2`;
alter table `athletics_rank` add column `eCanAttack4` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eCanAttack3`;
alter table `athletics_rank` add column `eCanAttack5` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eCanAttack4`;

alter table `athletics_rank` add column `eRivalType1` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eCanAttack5`;
alter table `athletics_rank` add column `eRivalType2` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eRivalType1`;
alter table `athletics_rank` add column `eRivalType3` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eRivalType2`;
alter table `athletics_rank` add column `eRivalType4` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eRivalType3`;
alter table `athletics_rank` add column `eRivalType5` tinyint(3) unsigned NOT NULL DEFAULT '0' after `eRivalType4`;

