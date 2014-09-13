CREATE TABLE `titlepaper` (
    `titleid` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `award` varchar(255) NOT NULL DEFAULT '0',
    `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `option_num` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`titleid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
