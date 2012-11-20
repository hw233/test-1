CREATE TABLE IF NOT EXISTS `arena_extra_board` (
    `week` tinyint(3) NOT NULL,
    `sufferTotal` int(10) unsigned NOT NULL,
    `playerId1` bigint(20) unsigned NOT NULL,
    `playerId2` bigint(20) unsigned NOT NULL,
    `playerId3` bigint(20) unsigned NOT NULL,
    `playerId4` bigint(20) unsigned NOT NULL,
    `playerId5` bigint(20) unsigned NOT NULL,
    `sufferCnt1` int(10) unsigned NOT NULL,
    `sufferCnt2` int(10) unsigned NOT NULL,
    `sufferCnt3` int(10) unsigned NOT NULL,
    `sufferCnt4` int(10) unsigned NOT NULL,
    `sufferCnt5` int(10) unsigned NOT NULL,
    `rank1` tinyint(3) NOT NULL,
    `rank2` tinyint(3) NOT NULL,
    `rank3` tinyint(3) NOT NULL,
    `rank4` tinyint(3) NOT NULL,
    `rank5` tinyint(3) NOT NULL,
    PRIMARY KEY (`week`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


