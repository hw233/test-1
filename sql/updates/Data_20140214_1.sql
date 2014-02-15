DROP TABLE IF EXISTS `ride_cangjian`;
CREATE TABLE `ride_cangjian` (
    `floor` tinyint(3) unsigned NOT NULL,
    `name` varchar(255) NOT NULL DEFAULT '',
    `prob` int(10) unsigned NOT NULL,
    `otherNum` int(10) unsigned NOT NULL,
    `bless` varchar(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`floor`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

