function Collect_00004280()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(31401) then
		if not player:GetPackage():ExistItem(15071) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15071);
		end
	end
	return action;
end



function Collect_00004280_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(31401) then
		if not player:GetPackage():ExistItem(15071) and player:GetPackage():AddItem(15071,1,1) then
			task:AddTaskStep(31401);
			return true;
		end
	end
	return false;
end