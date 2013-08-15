alter table friend drop key id_friendId;
alter table friend add unique key id_type_friendId(`id`,`type`,`friendId`);
