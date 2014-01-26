alter table modify_mount add `curfloor` tinyint(3) unsigned NOT NULL DEFAULT '0' after `chips`;
alter table modify_mount add `failtimes` int(10) unsigned NOT NULL DEFAULT '0' after `curfloor`;
