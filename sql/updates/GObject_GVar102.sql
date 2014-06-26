replace into gvar(id , data , over ) values(102,0,4294967295);
update gvar set data = (select max(level) from fighter where id < 10 ) where id = 102;
