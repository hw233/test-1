RENAME  TABLE `athletics_rank1` TO `athletics_rank`;
ALTER TABLE `athletics_rank` ADD COLUMN `row` tinyint(1) NOT NULL default '0'  FIRST;
INSERT INTO `athletics_rank`(`row`, `rank`, `ranker`, `maxRank`, `challengeNum`, `challengeTime`, `boxId`, `boxFlushTime`, `winStreak`)  SELECT 1, rank, ranker, maxRank, challengeNum, challengeTime, boxId, boxFlushTime, winStreak FROM `athletics_rank2`;
DROP TABLE  `athletics_rank2`;