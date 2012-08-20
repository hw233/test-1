CREATE TABLE   IF NOT EXISTS  `eupgrade`(
    `eqid` int(10) unsigned NOT NULL,
    `stuff` varchar(512) NOT NULL DEFAULT '',
    `targetid` int(10) unsigned NOT NULL,
    PRIMARY KEY (`eqid`)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
