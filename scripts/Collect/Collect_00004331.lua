function Collect_00004331()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(20403) then
		if not player:GetPackage():ExistItem(15020) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15020);
		end
	end
	return action;
end



function Collect_00004331_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(20403) then
		if not player:GetPackage():ExistItem(15020) and player:GetPackage():AddItem(15020,1,1) then
			task:AddTaskStep(20403);
			return true;
		end
	end
	return false;
end