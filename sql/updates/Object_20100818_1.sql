/*
MySQL Data Transfer
Source Host: 192.168.4.3
Source Database: szd_hohh
Target Host: 192.168.4.3
Target Database: szd_hohh
Date: 2010/8/18 11:50:18
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for trade
-- ----------------------------
CREATE TABLE `trade` (
  `tradeId` int(10) NOT NULL,
  `ownerId` bigint(20) NOT NULL,
  `tradeSide` tinyint(1) NOT NULL COMMENT '0:交易方 1:被交易方',
  `tradeStatus` tinyint(1) NOT NULL,
  `tradeName` varchar(255) NOT NULL,
  `tradeTitle` varchar(255) NOT NULL default '',
  `tradeTime` int(10) NOT NULL,
  `coin` int(10) NOT NULL default '0',
  `gold` int(10) NOT NULL default '0',
  `items` varchar(255) NOT NULL default ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
