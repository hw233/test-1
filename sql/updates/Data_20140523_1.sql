DROP TABLE IF EXISTS `newquestions`;
CREATE TABLE `newquestions` (
    `id` smallint(6) unsigned NOT NULL,
    `lvl` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `answer` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
