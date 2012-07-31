ALTER TABLE `activityData` ADD COLUMN `scores` int(10) unsigned NOT NULL after `flags`;
ALTER TABLE `activityData` ADD COLUMN `propsID` int(10) unsigned NOT NULL after `scores`;
