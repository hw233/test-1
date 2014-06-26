
replace into var (id, playerId, data, over) select 817, a.id, b.data, 4294967295 from player a ,var b where b.id = 599 and a.id = b.playerId;
