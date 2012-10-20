alter table `skills` add column `color` tinyint(3) unsigned NOT NULL DEFAULT '1' after `name`;
alter table `formation` add column `battlePoint` smallint(5) unsigned NOT NULL DEFAULT '0' after `name`;

