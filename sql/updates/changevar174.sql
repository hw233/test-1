update var set data = 0 where id = 174;
update var as a, var as b set a.data = 1 where a.id = 174 and a.playerId = b.playerId and b.id = 173 and b.data > 0 and b.over >= 1394899200;
