ALTER TABLE `clan_player` DROP COLUMN `battleClanId`, DROP COLUMN `battleClanTime`, DROP COLUMN `battleClanHold`;
ALTER IGNORE TABLE `clan_player`  ADD PRIMARY KEY(`playerId`); 
ALTER TABLE `clan` CHANGE COLUMN `battleStatus`  `battleStatus` TINYINT(1)  UNSIGNED NOT NULL; 
