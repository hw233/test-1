CREATE TABLE `invitegoback` (
   `inviteeId` bigint(20) unsigned NOT NULL DEFAULT '0',
   `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
    unique KEY invitee_player_id(`inviteeId`,`playerId`),
    INDEX (`inviteeId`),
    INDEX (`playerId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;
