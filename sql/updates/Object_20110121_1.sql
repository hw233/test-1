SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `exchange_ticket`
-- ----------------------------
DROP TABLE IF EXISTS `exchange_ticket`;
CREATE TABLE `exchange_ticket` (
  `playerId` bigint(20) unsigned NOT NULL,
  `exchange_num` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

