require("scripts/global")
require("scripts/Monster/Monster_00005000")
require("scripts/Monster/Monster_00005001")
require("scripts/Monster/Monster_00005002")
require("scripts/Monster/Monster_00005003")
require("scripts/Monster/Monster_00005004")
require("scripts/Monster/Monster_00005005")
require("scripts/Monster/Monster_00005006")
require("scripts/Monster/Monster_00005008")
require("scripts/Monster/Monster_00005009")
require("scripts/Monster/Monster_00005010")
require("scripts/Monster/Monster_00005011")
require("scripts/Monster/Monster_00005012")
require("scripts/Monster/Monster_00005013")
require("scripts/Monster/Monster_00005016")
require("scripts/Monster/Monster_00005017")
require("scripts/Monster/Monster_00005018")
require("scripts/Monster/Monster_00005019")
require("scripts/Monster/Monster_00005020")
require("scripts/Monster/Monster_00005021")
require("scripts/Monster/Monster_00005022")
require("scripts/Monster/Monster_00005025")
require("scripts/Monster/Monster_00005032")


local Monster_Function_Table = {
	[5000] = Monster_00005000,
	[5001] = Monster_00005001,
	[5002] = Monster_00005002,
	[5003] = Monster_00005003,
	[5004] = Monster_00005004,
	[5005] = Monster_00005005,
	[5006] = Monster_00005006,
	[5008] = Monster_00005008,
	[5009] = Monster_00005009,
	[5010] = Monster_00005010,
	[5011] = Monster_00005011,
	[5012] = Monster_00005012,
	[5013] = Monster_00005013,
	[5016] = Monster_00005016,
	[5017] = Monster_00005017,
	[5018] = Monster_00005018,
	[5019] = Monster_00005019,
	[5020] = Monster_00005020,
	[5021] = Monster_00005021,
	[5022] = Monster_00005022,
	[5025] = Monster_00005025,
	[5032] = Monster_00005032,
};


function RunMonsterKilled(monsterId, monsterNum)
	if Monster_Function_Table[monsterId] == nil then
		return false;
	end
	Monster_Function_Table[monsterId](monsterNum);
	return true;
end