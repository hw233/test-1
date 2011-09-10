
update athletics_rank a, player b, fighter c set a.row=1 where a.ranker=b.id and b.id=c.playerId and c.level>44;

