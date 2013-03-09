
alter table `player` add column `canHirePet` varchar(255) NOT NULL DEFAULT '' after `openid`;
delete from var where id = 296 or id = 297 or id = 298;

DROP TABLE IF EXISTS `fairyPet`;
CREATE TABLE `fairyPet` (                       
    `id` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,  
    `petLev` smallint(6) unsigned NOT NULL,
    `petBone` smallint(6) unsigned NOT NULL,
    `pinjieBless` smallint(6) unsigned NOT NULL,      
    `genguBless` int(10) unsigned NOT NULL,        
    `chong` smallint(6) unsigned NOT NULL,        
    `onBattle` tinyint(3) unsigned NOT NULL,      
    `overTime` int(10) unsigned NOT NULL,
    `xiaozhou` smallint(6) unsigned NOT NULL,
    `dazhou` smallint(6) unsigned NOT NULL,
    PRIMARY KEY (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
