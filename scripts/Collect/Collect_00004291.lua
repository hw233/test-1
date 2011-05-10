function Collect_00004291()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(31904) then
		if not player:GetPackage():ExistItem(15082) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15082);
		end
	end
	return action;
end



function Collect_00004291_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(31904) then
		if not player:GetPackage():ExistItem(15082) and player:GetPackage():AddItem(15082,1,1) then
			task:AddTaskStep(31904);
			return true;
		end
	end
	return false;
end