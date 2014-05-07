alter table `fighter` add `lingshi` varchar(255) NOT NULL after `cittas`;

CREATE TABLE IF NOT EXISTS `lingshiAttr` (
   `id` int(10) unsigned NOT NULL DEFAULT '0',
   `itemId`  int(10) unsigned NOT NULL DEFAULT '0',
   `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
   `exp`  int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;
