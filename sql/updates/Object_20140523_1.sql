CREATE TABLE IF NOT EXISTS `questions` (
    `answerId` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `questionsId` smallint(5) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`answerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
