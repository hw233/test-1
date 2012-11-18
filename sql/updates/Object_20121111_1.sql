ALTER TABLE `activityData` DROP COLUMN `awardID`;
ALTER TABLE `activityData` DROP COLUMN `point`;
ALTER TABLE `activityData` DROP COLUMN award;
ALTER TABLE `activityData` DROP COLUMN `flags`;
ALTER TABLE `activityData` ADD COLUMN `signRecord` varchar(1024) NOT NULL DEFAULT '' after `propsID`;
