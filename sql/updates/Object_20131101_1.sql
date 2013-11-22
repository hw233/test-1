DROP TABLE IF EXISTS `player_qishiban`;
CREATE TABLE IF NOT EXISTS `player_qishiban` (
    `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    `guankaId` int(10) unsigned NOT NULL DEFAULT '0',
    `score` int(10) unsigned NOT NULL DEFAULT '0',
    `beginTime` int(10) unsigned NOT NULL DEFAULT '0',
    `endTime` int(10) unsigned NOT NULL DEFAULT '0',
    `awardMark` int(10) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY(`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
