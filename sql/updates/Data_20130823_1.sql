alter table dungeon_level add column `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0';
alter table dungeon_level add unique key id_difficulty(`id`,`level`,`difficulty`);
alter table dungeon_level drop KEY id_level ;

