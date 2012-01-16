update clan as c, clan_tmp as t set c.battleScore = CASE WHEN c.battleScore >= t.battleScore THEN c.battleScore - t.battleScore ELSE 0 END where t.id = c.id;
