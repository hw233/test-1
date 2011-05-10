/*
MySQL Data Transfer
Source Host: 192.168.4.3
Source Database: szd_hohh
Target Host: 192.168.4.3
Target Database: szd_hohh
Date: 2010/9/14 18:29:29
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for athletics_rank1
-- ----------------------------
CREATE TABLE `athletics_rank1` (
  `rank` int(10) NOT NULL default '0',
  `ranker` bigint(20) NOT NULL default '0',
  `maxRank` smallint(4) NOT NULL default '0',
  `challengeNum` tinyint(1) NOT NULL default '0',
  `challengeTime` int(10) NOT NULL default '0',
  `boxId` smallint(4) NOT NULL default '0',
  `boxFlushTime` int(10) NOT NULL default '0',
  `winStreak` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`ranker`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for athletics_rank2
-- ----------------------------
CREATE TABLE `athletics_rank2` (
  `rank` int(10) NOT NULL default '0',
  `ranker` bigint(20) NOT NULL default '0',
  `maxRank` smallint(4) NOT NULL default '0',
  `challengeNum` tinyint(1) NOT NULL default '0',
  `challengeTime` int(10) NOT NULL default '0',
  `boxId` smallint(4) NOT NULL default '0',
  `boxFlushTime` int(10) NOT NULL default '0',
  `winStreak` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`ranker`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for athletics_record
-- ----------------------------
CREATE TABLE `athletics_record` (
  `id` int(10) NOT NULL,
  `atker` bigint(20) NOT NULL,
  `defer` bigint(20) NOT NULL,
  `repid` int(10) NOT NULL,
  `time` int(10) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
