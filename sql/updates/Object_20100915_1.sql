ALTER TABLE `athletics_record`  ADD COLUMN `winSide` tinyint(1) NOT NULL default '0' AFTER `time`, ADD COLUMN `awardType` tinyint(1) NOT NULL default '0' AFTER `winSide`, ADD COLUMN `awardAtkerCount` int(10) NOT NULL default '0' AFTER `awardType`, ADD COLUMN `awardDeferCount` int(10) NOT NULL default '0' AFTER `awardAtkerCount`;

ALTER TABLE `athletics_rank1`  CHANGE COLUMN `rank` `rank` INT(10) NOT NULL AUTO_INCREMENT FIRST,  ADD INDEX `rank` (`rank`);
ALTER TABLE `athletics_rank1`  AUTO_INCREMENT=1;
INSERT INTO `athletics_rank1`(`ranker`, `boxId`) SELECT p.id, 1001 + floor(3 * Rand()) FROM `player` p, `fighter` f WHERE p.id = f.playerId AND p.mainFighter = f.id AND f.level >= 30 AND f.level < 51 ORDER BY f.experience DESC, p.created DESC;
UPDATE `athletics_rank1` SET `maxRank` = `rank`;
ALTER TABLE `athletics_rank1`  CHANGE COLUMN `rank` `rank` INT(10) NOT NULL DEFAULT '0' FIRST,  DROP INDEX `rank`;

ALTER TABLE `athletics_rank2`  CHANGE COLUMN `rank` `rank` INT(10) NOT NULL AUTO_INCREMENT FIRST,  ADD INDEX `rank` (`rank`);
ALTER TABLE `athletics_rank2`  AUTO_INCREMENT=1;
INSERT INTO `athletics_rank2`(`ranker`, `boxId`) SELECT p.id, 1001 + floor(3 * Rand()) FROM `player` p, `fighter` f WHERE p.id = f.playerId AND p.mainFighter = f.id AND f.level >= 51 ORDER BY f.experience DESC, p.created DESC;
UPDATE `athletics_rank2` SET `maxRank` = `rank`;
ALTER TABLE `athletics_rank2`  CHANGE COLUMN `rank` `rank` INT(10) NOT NULL DEFAULT '0' FIRST,  DROP INDEX `rank`;
