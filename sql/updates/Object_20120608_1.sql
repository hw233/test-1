alter table `arena_bet` add column `state` tinyint(3) unsigned NOT NULL DEFAULT 0 after `round`;
alter table `arena_bet` add column `recieved` tinyint(3) unsigned NOT NULL DEFAULT 0 after `group`;
alter table `arena_bet` drop PRIMARY KEY;
alter table `arena_bet` add PRIMARY KEY (`id`,`state`,`round`,`group`, `pos`);
alter table `second_soul` add column `xinxiu` tinyint(3) unsigned NOT NULL DEFAULT 0 after `cls`;
