function Collect_00004244()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(30803) then
		if not player:GetPackage():ExistItem(15063) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15063);
		end
	end
	return action;
end



function Collect_00004244_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(30803) then
		if not player:GetPackage():ExistItem(15063) and player:GetPackage():AddItem(15063,1,1) then
			task:AddTaskStep(30803);
			return true;
		end
	end
	return false;
end