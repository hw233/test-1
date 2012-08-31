CREATE TABLE `elixir` (
    `id` int(10) unsigned NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `strength` int(5) NOT NULL DEFAULT '0', 
    `physique` int(5) NOT NULL DEFAULT '0', 
    `agility` int(5) NOT NULL DEFAULT '0', 
    `intelligence` int(5) NOT NULL DEFAULT '0', 
    `will` int(5) NOT NULL DEFAULT '0', 
    `soul` int(5) NOT NULL DEFAULT '0', 
    UNIQUE KEY `id_playerId` (`id`,`playerId`),
    KEY `playerId` (`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
