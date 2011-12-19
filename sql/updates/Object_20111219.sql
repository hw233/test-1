
CREATE TABLE IF NOT EXISTS `money` (
    `time` int(10) unsigned NOT NULL,
    `type` tinyint(3) unsigned NOT NULL,
    `gold` int(10) unsigned NOT NULL,
    `coupon` int(10) unsigned NOT NULL,
    `tael` int(10) unsigned NOT NULL,
    `achievement` int(10) unsigned NOT NULL,
    `prestige` int(10) unsigned NOT NULL,
    PRIMARY KEY (`time`, `type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
