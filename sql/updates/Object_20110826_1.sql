
update athletics_rank a, player b, fighter c set a.row = 0 where a.ranker=b.id and b.id=c.playerid and c.level < 46;

update athletics_rank a, player b, fighter c set a.row = 1 where a.ranker=b.id and b.id=c.playerid and c.level > 45;

update player set shimen='',fshimen='',yamen='',fyamen='';
