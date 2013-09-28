/*天书奇缘 */
CREATE TABLE `AirBookData` (
   `playerId` bigint(20) unsigned NOT NULL,
   `overTime` int(10) unsigned NOT NULL,
   `grade`  int(10) unsigned NOT NULL,
   `recharge`  int(10) unsigned NOT NULL,
   `consume`  int(10) unsigned NOT NULL,
   `flags`  varchar(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`playerId`,`overTime`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;
