require("scripts/global")
require("scripts/Monster/Monster_00005000")
require("scripts/Monster/Monster_00005001")


local Monster_Function_Table = {
	[5000] = Monster_00005000,
	[5001] = Monster_00005001,
};


function RunMonsterKilled(monsterId, monsterNum)
	if Monster_Function_Table[monsterId] == nil then
		return false;
	end
	Monster_Function_Table[monsterId](monsterNum);
	return true;
end