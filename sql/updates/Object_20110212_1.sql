UPDATE `player` SET `slaught` = 0;
ALTER TABLE `player` CHANGE `slaught` `title` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0';
