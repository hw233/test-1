require("global")
require("Collect/Collect_00004280")
require("Collect/Collect_00004226")
require("Collect/Collect_00004279")
require("Collect/Collect_00004139")
require("Collect/Collect_00004186")
require("Collect/Collect_00004329")
require("Collect/Collect_00004330")
require("Collect/Collect_00004244")
require("Collect/Collect_00004291")
require("Collect/Collect_00004120")
require("Collect/Collect_00004165")
require("Collect/Collect_00004096")
require("Collect/Collect_00004096")
require("Collect/Collect_00004262")


local Collect_Function_Table = {
		[4280] = Collect_00004280,
		[4226] = Collect_00004226,
		[4279] = Collect_00004279,
		[4139] = Collect_00004139,
		[4186] = Collect_00004186,
		[4329] = Collect_00004329,
		[4330] = Collect_00004330,
		[4244] = Collect_00004244,
		[4291] = Collect_00004291,
		[4120] = Collect_00004120,
		[4165] = Collect_00004165,
		[4096] = Collect_00004096,
		[4096] = Collect_00004096,
		[4262] = Collect_00004262,
	};


function RunCollectTask(npcId)
	if Collect_Function_Table[npcId] == nil then
		return ActionTable:Instance();
	end
	return Collect_Function_Table[npcId]();	
end

--采集动作
local Collect_Item_Function_Table = {
		[4280] = Collect_00004280_Item_Action,
		[4226] = Collect_00004226_Item_Action,
		[4279] = Collect_00004279_Item_Action,
		[4139] = Collect_00004139_Item_Action,
		[4186] = Collect_00004186_Item_Action,
		[4329] = Collect_00004329_Item_Action,
		[4330] = Collect_00004330_Item_Action,
		[4244] = Collect_00004244_Item_Action,
		[4291] = Collect_00004291_Item_Action,
		[4120] = Collect_00004120_Item_Action,
		[4165] = Collect_00004165_Item_Action,
		[4096] = Collect_00004096_Item_Action,
		[4096] = Collect_00004096_Item_Action,
		[4262] = Collect_00004262_Item_Action,
	};


function RunCollectTaskItem(npcId)
	if Collect_Item_Function_Table[npcId] == nil then
		return false;
	end
	return Collect_Item_Function_Table[npcId]();
end