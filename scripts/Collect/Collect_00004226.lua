function Collect_00004226()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(30407) then
		if not player:GetPackage():ExistItem(15038) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15038);
		end
	end
	return action;
end



function Collect_00004226_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(30407) then
		if not player:GetPackage():ExistItem(15038) and player:GetPackage():AddItem(15038,1,1) then
			task:AddTaskStep(30407);
			return true;
		end
	end
	return false;
end