update var set data = data + 151587092 where (id = 702 or id = 703) and playerId = 36591746973869731;
update var set data = data + 150994944 where id = 705 and playerId = 36591746973869731;

update var set data = data + 117901072 where (id = 702 or id = 703) and playerId = 40813871641208012;

alter table modify_mount add `curfloor` tinyint(3) unsigned NOT NULL DEFAULT '0' after `chips`;
alter table modify_mount add `curfloor1` tinyint(3) unsigned NOT NULL DEFAULT '0' after `curfloor`;
alter table modify_mount add `failtimes` int(10) unsigned NOT NULL DEFAULT '0' after `curfloor1`;
