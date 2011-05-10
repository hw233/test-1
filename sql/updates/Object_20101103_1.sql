/*
MySQL Data Transfer
Source Host: 192.168.4.3
Source Database: szd_hohh
Target Host: 192.168.4.3
Target Database: szd_hohh
Date: 2010/11/3 10:15:40
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for fighter_train
-- ----------------------------
CREATE TABLE `fighter_train` (
  `fgtId` int(11) NOT NULL,
  `ownerId` int(11) NOT NULL,
  `priceType` tinyint(4) NOT NULL,
  `price` smallint(6) NOT NULL,
  `trainTime` int(11) NOT NULL,
  `trainRemain` int(11) NOT NULL,
  `checkTime` int(11) NOT NULL,
  `accExp` int(11) NOT NULL default '0',
  PRIMARY KEY  (`fgtId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
