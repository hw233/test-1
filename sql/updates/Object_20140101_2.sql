CREATE TABLE `petteamcopylog` (
    `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
    `playerName` varchar(255) NOT NULL,
    `monsterName` varchar(255) NOT NULL,
    `items` varchar(512) NOT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

