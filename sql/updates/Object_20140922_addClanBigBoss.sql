alter table `clanbigboss` add `player1` bigint(20) unsigned NOT NULL DEFAULT '0' after `matk`;
alter table `clanbigboss` add `player2` bigint(20) unsigned NOT NULL DEFAULT '0' after `player1`;
