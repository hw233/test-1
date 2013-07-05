
CREATE TABLE IF NOT EXISTS `clancity` (
    `id`   tinyint(3) unsigned NOT NULL DEFAULT 1,
    `type` tinyint(3) unsigned NOT NULL,
    `round` tinyint(3) unsigned NOT NULL,
    `openTime` int(10) unsigned NOT NULL,
    `defClanId` int(10) unsigned NOT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `clancity_spot` (
    `id`   tinyint(3) unsigned NOT NULL,
    `hp`   smallint(5) unsigned NOT NULL,
    `canAtk` tinyint(3) unsigned NOT NULL,
    `skill_dmg0` tinyint(3) unsigned NOT NULL,
    `skill_action0` tinyint(3) unsigned NOT NULL,
    `skill_bomb0` tinyint(3) unsigned NOT NULL,
    `skill_dmg1` tinyint(3) unsigned NOT NULL,
    `skill_action1` tinyint(3) unsigned NOT NULL,
    `skill_bomb1` tinyint(3) unsigned NOT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `clancity_clan` (
    `clanId`   int(10) unsigned NOT NULL,
    `score`   int(10) unsigned NOT NULL,
    `skill_hp` tinyint(3) unsigned NOT NULL,
    `skill_atk` tinyint(3) unsigned NOT NULL,
    `skill_action` tinyint(3) unsigned NOT NULL,
    PRIMARY KEY(`clanId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `clancity_player` (
    `playerId`   bigint(20) unsigned NOT NULL,
    `side`   tinyint(3) unsigned NOT NULL,
    `entered`   tinyint(3) unsigned NOT NULL,
    `win`   tinyint(3) unsigned NOT NULL,
    `dead`   tinyint(3) unsigned NOT NULL,
    `realive`   tinyint(3) unsigned NOT NULL,
    `score` int(10) unsigned NOT NULL,
    `realivecd` int(10) unsigned NOT NULL,
    `weary` int(10) unsigned NOT NULL,
    PRIMARY KEY(`playerId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



