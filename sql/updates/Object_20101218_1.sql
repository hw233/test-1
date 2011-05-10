CREATE TABLE `clan_temp` ENGINE=Memory SELECT `id` FROM `clan` WHERE not `id` IN (SELECT `clanId` FROM `clan_skill`);
INSERT INTO `clan_skill` SELECT `id`, 1, 1, 0 FROM `clan_temp`;
DROP TABLE `clan_temp`;
