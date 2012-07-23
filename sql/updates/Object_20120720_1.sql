ALTER TABLE `activityData` ADD `scores` int(10) unsigned NOT NULL after `flags`;
ALTER TABLE `activityData` ADD `propsID` int(10) unsigned NOT NULL after `scores`;
