UPDATE `clan` SET `allyClan1` = 0;
ALTER TABLE `clan` DROP COLUMN `allyClan2`, DROP COLUMN `allyClan3`, CHANGE COLUMN `allyClan1` `allyClan` INT(10) UNSIGNED NOT NULL DEFAULT '0';