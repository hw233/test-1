alter table player_frontmap drop key `player_id`;
alter table player_frontmap add unique key `player_id_spot` (`playerId`,`id`, `spot`);
alter table player_frontmap add key `playerIdId` (`playerId`, `id`);
