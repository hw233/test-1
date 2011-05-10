-- ----------------------------
-- Table structure for `pass_word`
-- ----------------------------
DROP TABLE IF EXISTS `pass_word`;
CREATE TABLE `pass_word` (
  `playerId` bigint(20) unsigned NOT NULL,
  `password` varchar(255) NOT NULL,
  `questionForPwd` varchar(255) NOT NULL,
  `answerForPwd` varchar(255) NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;