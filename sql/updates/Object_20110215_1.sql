UPDATE `fighter` SET `skill` = (`skill` DIV 10) * 100 + (`skill` % 10) WHERE `skill` > 0 AND `skill` < 10000;
ALTER TABLE `equipment` ADD COLUMN `attrType3` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `attrValue2`, ADD COLUMN `attrValue3` SMALLINT(6) NOT NULL DEFAULT '0' AFTER `attrType3`;
ALTER TABLE `player` MODIFY COLUMN `icCount` VARCHAR(32) NOT NULL DEFAULT '', ADD COLUMN `bookStore` VARCHAR(255) NOT NULL DEFAULT '0' AFTER `tavernId`, ADD COLUMN `bossLevel` smallint(4) UNSIGNED NOT NULL DEFAULT '21' AFTER `lineup`;
