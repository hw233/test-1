
alter table `player` add column `qqawardgot` tinyint(1) unsigned NOT NULL DEFAULT '0' after `qqvipyear`;
alter table `player` add column  `qqawardEnd` int(10) unsigned NOT NULL DEFAULT '0' after `qqawardgot`;
alter table `player` add column  `ydGemId` int(10) unsigned NOT NULL DEFAULT '0' after `qqawardEnd`;
