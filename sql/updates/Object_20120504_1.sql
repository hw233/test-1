alter table `wboss` add column `hp` int(10) unsigned NOT NULL after `last`;
alter table `wboss` add column `atk` int(10) unsigned NOT NULL after `hp`;
alter table `wboss` add column `matk` int(10) unsigned NOT NULL after `atk`;
