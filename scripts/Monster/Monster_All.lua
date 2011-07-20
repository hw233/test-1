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
require("scripts/Monster/Monster_00005023")
require("scripts/Monster/Monster_00005025")
require("scripts/Monster/Monster_00005032")
require("scripts/Monster/Monster_00005049")
require("scripts/Monster/Monster_00005054")
require("scripts/Monster/Monster_00005055")
require("scripts/Monster/Monster_00005056")
require("scripts/Monster/Monster_00005057")
require("scripts/Monster/Monster_00005058")
require("scripts/Monster/Monster_00005059")
require("scripts/Monster/Monster_00005071")
require("scripts/Monster/Monster_00005088")
require("scripts/Monster/Monster_00005089")
require("scripts/Monster/Monster_00005091")
require("scripts/Monster/Monster_00005092")
require("scripts/Monster/Monster_00005093")
require("scripts/Monster/Monster_00005094")
require("scripts/Monster/Monster_00005095")
require("scripts/Monster/Monster_00005273")
require("scripts/Monster/Monster_00000001")
require("scripts/Monster/Monster_00005065")


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
	[5023] = Monster_00005023,
	[5025] = Monster_00005025,
	[5032] = Monster_00005032,
	[5049] = Monster_00005049,
	[5054] = Monster_00005054,
	[5055] = Monster_00005055,
	[5056] = Monster_00005056,
	[5057] = Monster_00005057,
	[5058] = Monster_00005058,
	[5059] = Monster_00005059,
	[5071] = Monster_00005071,
	[5088] = Monster_00005088,
	[5089] = Monster_00005089,
	[5091] = Monster_00005091,
	[5092] = Monster_00005092,
	[5093] = Monster_00005093,
	[5094] = Monster_00005094,
	[5095] = Monster_00005095,
	[5273] = Monster_00005273,
	[1] = Monster_00000001,
	[5065] = Monster_00005065,
};


function RunMonsterKilled(monsterId, monsterNum)
	if Monster_Function_Table[monsterId] == nil then
		return false;
	end
	Monster_Function_Table[monsterId](monsterNum);
	return true;
end