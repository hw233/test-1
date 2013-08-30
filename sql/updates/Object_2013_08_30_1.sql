alter table dungeon_player add column `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0';
alter table dungeon_player add unique key id_difficulty(`id`,`playerId`,`difficulty`);
alter table dungeon_player drop KEY id_playerId ;

