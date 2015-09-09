Drop TABLE IF EXISTS `globalPVPName`;
CREATE TABLE `globalPVPName` (
    `index` int(10) unsigned NOT NULL COMMENT '编号',
    `name` varchar(255) NOT NULL,
    PRIMARY KEY (`index`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;



