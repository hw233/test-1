CREATE TABLE `couplecopy` (
    `id` tinyint(3) unsigned NOT NULL,
    `name` varchar(255) NOT NULL DEFAULT '',
    `location` int(10) NOT NULL ,
    `monster` int(10)  NOT NULL ,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
