alter table `skillstrengthen_effect` add column `valueExtend1` float(5,2) unsigned NOT NULL DEFAULT '1.00' after `value`;
alter table `skillstrengthen_effect` add column `valueExtend2` float(5,2) unsigned NOT NULL DEFAULT '1.00' after `valueExtend1`;
