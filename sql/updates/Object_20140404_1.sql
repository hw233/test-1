CREATE TABLE IF NOT EXISTS `zhenyuanAttr` (
   `id` int(10) unsigned NOT NULL DEFAULT '0',
   `itemId`  int(10) unsigned NOT NULL DEFAULT '0',
   `zycolor` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `types` varchar(255) NOT NULL DEFAULT '',
   `values` varchar(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

alter table `player` add `zhenyuans` varchar(1024) NOT NULL DEFAULT '' after `formations`;
