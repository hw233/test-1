ALTER TABLE `player_jiguanyu` DROP PRIMARY KEY;
ALTER TABLE `player_jiguanyu` ADD PRIMARY KEY(`playerId`, `scheme`, `jiguanId`);
