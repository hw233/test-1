require("scripts/global")



local Explore_Function_Table = {
};


function RunExplore(npcId)
	if Explore_Function_Table[npcId] == nil then
		return false;
	end
	return Explore_Function_Table[npcId]();
end