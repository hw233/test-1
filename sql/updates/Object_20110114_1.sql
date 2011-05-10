SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `luckydrawgold`
-- ----------------------------
DROP TABLE IF EXISTS `luckydrawgold`;
CREATE TABLE `luckydrawgold` (
  `playerId` bigint(20) unsigned NOT NULL,
  `cost` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

