CREATE TABLE IF NOT EXISTS `real_item_award` (
    `id` int(10) unsigned NOT NULL,
    `cd` int(10) unsigned NOT NULL DEFAULT '0',
    `card_no` varchar(255) NOT NULL,
    `card_psw` varchar(255) NOT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
