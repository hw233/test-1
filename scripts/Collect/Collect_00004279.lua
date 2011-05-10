function Collect_00004279()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(31403) then
		if not player:GetPackage():ExistItem(15073) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15073);
		end
	end
	return action;
end



function Collect_00004279_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(31403) then
		if not player:GetPackage():ExistItem(15073) and player:GetPackage():AddItem(15073,1,1) then
			task:AddTaskStep(31403);
			return true;
		end
	end
	return false;
end