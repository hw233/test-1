DROP TABLE IF EXISTS `sendbeans_log`;
CREATE TABLE IF NOT EXISTS `sendbeans_log` (
     `senderId` bigint(20) unsigned NOT NULL DEFAULT '0',
     `receiverId` bigint(20) unsigned NOT NULL DEFAULT '0',
     `data` int(10) unsigned NOT NULL,
     `count` int(10) unsigned NOT NULL DEFAULT '0',
     `beantype`  tinyint(3) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

alter table `player` add column `announcement` varchar(1024) NOT NULL DEFAULT '';
