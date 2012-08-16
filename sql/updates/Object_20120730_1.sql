alter table `discount` drop column `discount`;
alter table `discount` add column `timeBegin` int(10) unsigned NOT NULL DEFAULT '0'after `itemid`;
alter table `discount` add column `timeEnd` int(10) unsigned NOT NULL DEFAULT '0'after `timeBegin`;
alter table `discount` add column `priceOriginal` int(10) unsigned NOT NULL DEFAULT '0'after `timeEnd`;
alter table `discount` add column `priceDiscount` int(10) unsigned NOT NULL DEFAULT '0'after `priceOriginal`;
alter table `discount` add column `type` tinyint(3) unsigned NOT NULL DEFAULT '0'after `priceDiscount`;
alter table `discount` add column `count` int(10) unsigned NOT NULL DEFAULT '0'after `type`;
