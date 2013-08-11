CREATE TABLE IF NOT EXISTS `clan_sptr` (
  `clanId` int(10) unsigned NOT NULL,
  `exp` int(10) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `refreshTimes` int(10) unsigned NOT NULL DEFAULT '0',
  `color` int(10) unsigned NOT NULL DEFAULT '0',
  `endTime` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY(`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

alter TABLE `clan_player` add column `activepoint` int(11) unsigned NOT NULL DEFAULT '0' after `proffer`;
alter TABLE `clan_player` add column `last_actpt` int(11) unsigned NOT NULL DEFAULT '0' after `activepoint`;
alter TABLE `clan_player` add column `actpt_endtime` int(11) unsigned NOT NULL DEFAULT '0' after `last_actpt`;

alter TABLE `clan_donate_record` add column `donateTo` tinyint(3) unsigned NOT NULL DEFAULT '0' after `donateName`;
alter TABLE `clan_donate_record` add column `donateType` tinyint(3) unsigned NOT NULL DEFAULT '0' after `donateTo`;
alter TABLE `clan_donate_record` drop column `techId`;
update `clan_donate_record` set `donateTo` = 1, `donateType` = 1 where `donateTo` = 0;

