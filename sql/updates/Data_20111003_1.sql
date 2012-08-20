alter table `npc_group` add column `lootId1` varchar(255) NOT NULL DEFAULT '' after `lootId`;
alter table `npc_group` add column `lootId2` varchar(255) NOT NULL DEFAULT '' after `lootId1`;
alter table `npc_group` add column `lootId3` varchar(255) NOT NULL DEFAULT '' after `lootId2`;
