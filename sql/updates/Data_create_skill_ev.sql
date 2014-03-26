CREATE TABLE IF NOT EXISTS `skill_ev` (
    `lev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    `effect` int(10) NOT NULL DEFAULT 0,
    `consume` int(10) unsigned NOT NULL DEFAULT 0,
    `needLev` tinyint(3) unsigned NOT NULL DEFAULT 0,
    PRIMARY KEY (`lev`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
