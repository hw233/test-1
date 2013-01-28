alter TABLE `discount` DROP COLUMN discount exValue int(10) unsigned NOT NULL;
alter TABLE `discount` ADD COLUMN `exType` tinyint(3) unsigned NOT NULL after `itemid`;
alter TABLE `discount` ADD COLUMN `exValue` int(10) unsigned NOT NULL after `exType`;
