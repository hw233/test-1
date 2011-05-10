DROP PROCEDURE IF EXISTS `new_player`;
DROP PROCEDURE IF EXISTS `new_mail`;
DELIMITER ;;
CREATE PROCEDURE `new_player`(IN `id` BIGINT(20) UNSIGNED, IN `name` VARCHAR(255), IN `country` INT(1) UNSIGNED, IN `location` INT(5) UNSIGNED, IN `fighterId` INT(10) UNSIGNED)  LANGUAGE SQL  NOT DETERMINISTIC  CONTAINS SQL  SQL SECURITY DEFINER  COMMENT '' BEGIN     INSERT INTO `player` (`id`, `name`, `country`, `location`, `mainFighter`, `lineup1`, `lineup2`, `lineup3`) VALUES (id, name, country, location, fighterId, CONCAT(fighterId, ',12'), CONCAT(fighterId, ',12'), CONCAT(fighterId, ',12')); END;;
CREATE PROCEDURE `new_mail`(IN `id` INT(10) UNSIGNED, IN `playerId` BIGINT(20) UNSIGNED, IN `sender` VARCHAR(255), IN `recvTime` INT(10) UNSIGNED, IN `flag` TINYINT(3) UNSIGNED, IN `title` VARCHAR(255), IN `content` VARCHAR(8192), IN `additional` INT(10) UNSIGNED)  LANGUAGE SQL  NOT DETERMINISTIC  CONTAINS SQL  SQL SECURITY DEFINER  COMMENT '' BEGIN  INSERT INTO `mail` (`mailId`, `playerId`, `sender`, `recvTime`, `flag`, `title`, `content`, `additionalId`) VALUES (id, playerId, sender, recvTime, flag, title, content, additional); END;;
DELIMITER ;    
