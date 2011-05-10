/*
MySQL Data Transfer
Source Host: 192.168.4.3
Source Database: szd_hohh
Target Host: 192.168.4.3
Target Database: szd_hohh
Date: 2010/8/26 15:27:49
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for sale
-- ----------------------------
CREATE TABLE `sale` (
  `saleId` int(10) NOT NULL,
  `ownerId` bigint(20) NOT NULL,
  `status` smallint(6) NOT NULL,
  `saleTime` int(10) NOT NULL,
  `priceType` int(10) NOT NULL default '0',
  `price` int(10) NOT NULL default '0',
  `itemId` int(10) NOT NULL,
  `itemNum` int(10) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
