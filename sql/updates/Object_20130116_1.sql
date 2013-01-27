alter TABLE `discount` ADD COLUMN `exType` tinyint(3) unsigned NOT NULL after `itemid`;
alter TABLE `discount` CHANGE discount exValue int(10) unsigned NOT NULL;
