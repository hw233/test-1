
update clan set construction=construction&0xFFFFFFFF;
update clan set level=1;
update athletics_rank a, player b set a.prestige=300 where (prestige&0xFFFF0000<>0) and a.ranker=b.id;
update athletics_rank a, player b set a.prestige=8000 where a.ranker=b.id and b.created  > '2011-09-15 12:00:00' and b.created  < '2011-09-16 12:37:07' and a.prestige > 8000;
update athletics_rank a, player b set a.prestige=8000 where a.ranker=b.id and b.created  > '2011-09-16 12:00:00' and b.created  < '2011-09-17 12:37:07' and a.prestige > 8000;

update athletics_rank a, player b set a.prestige=7200 where a.ranker=b.id and b.created  > '2011-09-17 12:00:00' and b.created  < '2011-09-18 12:37:07' and a.prestige > 7200;

update athletics_rank a, player b set a.prestige=6720 where a.ranker=b.id and b.created  > '2011-09-18 12:00:00' and b.created  < '2011-09-19 12:37:07' and a.prestige > 6720;

update athletics_rank a, player b set a.prestige=6240 where a.ranker=b.id and b.created  > '2011-09-19 12:00:00' and b.created  < '2011-09-18 12:37:07' and a.prestige > 6240;

update athletics_rank a, player b set a.prestige=5760 where a.ranker=b.id and b.created  > '2011-09-20 12:00:00' and b.created  < '2011-09-21 12:37:07' and a.prestige > 5760;

update athletics_rank a, player b set a.prestige=5280 where a.ranker=b.id and b.created  > '2011-09-21 12:00:00' and b.created  < '2011-09-22 12:37:07' and a.prestige > 5280;

update athletics_rank a, player b set a.prestige=3800 where a.ranker=b.id and b.created  > '2011-09-22 12:00:00' and b.created  < '2011-09-23 12:37:07' and a.prestige > 3800;

update athletics_rank a, player b set a.prestige=3320 where a.ranker=b.id and b.created  > '2011-09-23 12:00:00' and b.created  < '2011-09-24 12:37:07' and a.prestige > 3320;

update athletics_rank a, player b set a.prestige=2840 where a.ranker=b.id and b.created  > '2011-09-24 12:00:00' and b.created  < '2011-09-25 12:37:07' and a.prestige > 2840;

update athletics_rank a, player b set a.prestige=2360 where a.ranker=b.id and b.created  > '2011-09-25 12:00:00' and b.created  < '2011-09-26 12:37:07' and a.prestige > 2360;

update athletics_rank a, player b set a.prestige=1880 where a.ranker=b.id and b.created  > '2011-09-26 12:00:00' and b.created  < '2011-09-27 12:37:07' and a.prestige > 1880;
update athletics_rank a, player b set a.prestige=1400 where a.ranker=b.id and b.created  > '2011-09-27 12:00:00' and b.created  < '2011-09-28 12:37:07' and a.prestige > 1400;

update athletics_rank a, player b set a.prestige=960 where a.ranker=b.id and b.created  > '2011-09-28 12:00:00' and b.created  < '2011-09-29 12:37:07' and a.prestige > 960;

update athletics_rank a, player b set a.prestige=480 where a.ranker=b.id and b.created  > '2011-09-29 12:00:00' and a.prestige > 480;

