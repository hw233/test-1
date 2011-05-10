function Collect_00004186()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(60902) then
		if not player:GetPackage():ExistItem(15009) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15009);
		end
	end
	return action;
end



function Collect_00004186_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(60902) then
		if not player:GetPackage():ExistItem(15009) and player:GetPackage():AddItem(15009,1,1) then
			task:AddTaskStep(60902);
			return true;
		end
	end
	return false;
end