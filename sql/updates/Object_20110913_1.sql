
alter table `player` add column  `piccount` tinyint(2) unsigned NOT NULL DEFAULT '0' after `icCount`;
alter table `player` add column  `nextpicreset` int(10) unsigned NOT NULL DEFAULT '0' after `piccount`;

alter table `practice_data` drop column `hookadd`;
alter table `practice_data` drop column `nexthareset`;

