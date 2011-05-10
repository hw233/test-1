UPDATE `player` SET `lineup1` = `lineup2`, `formation1` = `formation2` WHERE `currentLineup` = 2;
UPDATE `player` SET `lineup1` = `lineup3`, `formation1` = `formation3` WHERE `currentLineup` = 3;
ALTER TABLE `player`
	CHANGE COLUMN `formation1` `formation` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `icCount`,
	CHANGE COLUMN `lineup1` `lineup` VARCHAR(255) NOT NULL DEFAULT '' AFTER `formation`,
	DROP COLUMN `currentLineup`,
	DROP COLUMN `formation2`,
	DROP COLUMN `lineup2`,
	DROP COLUMN `formation3`,
	DROP COLUMN `lineup3`;

DELIMITER ;;
/*!50003 DROP PROCEDURE IF EXISTS `new_player` */;;
/*!50003 CREATE*/ /*!50003 PROCEDURE `new_player`(IN `id` BIGINT(20) UNSIGNED, IN `name` VARCHAR(255), IN `country` INT(1) UNSIGNED, IN `location` INT(5) UNSIGNED, IN `fighterId` INT(10) UNSIGNED)
BEGIN     INSERT INTO `player` (`id`, `name`, `country`, `location`, `mainFighter`, `lineup`) VALUES (id, name, country, location, fighterId, CONCAT(fighterId, ',12')); END */;;
DELIMITER ;
