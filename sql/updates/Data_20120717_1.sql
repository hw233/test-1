
CREATE TABLE `skillstrengthenprob` (
    `id` smallint(5) unsigned NOT NULL DEFAULT 0,
    `prob1` int(10) unsigned NOT NULL DEFAULT 0,
    `prob2` int(10) unsigned NOT NULL DEFAULT 0,
    `prob3` int(10) unsigned NOT NULL DEFAULT 0,
    `prob4` int(10) unsigned NOT NULL DEFAULT 0,
    `prob5` int(10) unsigned NOT NULL DEFAULT 0,
    `prob6` int(10) unsigned NOT NULL DEFAULT 0,
    `prob7` int(10) unsigned NOT NULL DEFAULT 0,
    `prob8` int(10) unsigned NOT NULL DEFAULT 0,
    `prob9` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `skillstrengthen` (
    `id` smallint(5) unsigned NOT NULL DEFAULT 0,
    `exp1` int(10) unsigned NOT NULL DEFAULT 0,
    `exp2` int(10) unsigned NOT NULL DEFAULT 0,
    `exp3` int(10) unsigned NOT NULL DEFAULT 0,
    `exp4` int(10) unsigned NOT NULL DEFAULT 0,
    `exp5` int(10) unsigned NOT NULL DEFAULT 0,
    `exp6` int(10) unsigned NOT NULL DEFAULT 0,
    `exp7` int(10) unsigned NOT NULL DEFAULT 0,
    `exp8` int(10) unsigned NOT NULL DEFAULT 0,
    `exp9` int(10) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


