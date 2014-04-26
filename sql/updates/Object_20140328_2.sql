CREATE TABLE `applylist` (
   `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
   `applicantId` bigint(20) unsigned NOT NULL DEFAULT '0',
    unique KEY player_applicant_id(`playerId`,`applicantId`),
    INDEX (`playerId`),
    INDEX (`applicantId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

