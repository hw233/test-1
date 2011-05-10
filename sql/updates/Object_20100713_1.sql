/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DELIMITER ;;
/*!50003 DROP PROCEDURE IF EXISTS `clan_join` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50003 PROCEDURE `clan_join`(IN `clanId` INT(10) UNSIGNED, IN `pId` BIGINT(20) UNSIGNED, IN `cls` TINYINT(3) UNSIGNED, IN `optime` INT(10) UNSIGNED)
BEGIN
  DELETE FROM `clan_player` WHERE `playerId` = pId AND `class` = 16;
  INSERT INTO `clan_player` (`id`, `playerId`, `class`, `optime`) VALUES (clanId, pId, cls, optime);
END */;;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE*/;;
DELIMITER ;    
