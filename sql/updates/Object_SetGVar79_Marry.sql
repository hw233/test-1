REPLACE INTO gvar values(79,0,4294967295);
UPDATE gvar set data = (select count(*) from married_log)  where id =  79;
