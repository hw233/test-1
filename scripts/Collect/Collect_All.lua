require("scripts/global")


local Collect_Function_Table = {
	};


function RunCollectTask(npcId)
	if Collect_Function_Table[npcId] == nil then
		return ActionTable:Instance();
	end
	return Collect_Function_Table[npcId]();	
end

--采集动作
local Collect_Item_Function_Table = {
	};


function RunCollectTaskItem(npcId)
	if Collect_Item_Function_Table[npcId] == nil then
		return false;
	end
	return Collect_Item_Function_Table[npcId]();
end