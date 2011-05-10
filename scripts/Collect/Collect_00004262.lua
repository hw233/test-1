function Collect_00004262()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(31204) then
		if not player:GetPackage():ExistItem(15067) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15067);
		end
	end
	return action;
end



function Collect_00004262_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(31204) then
		if not player:GetPackage():ExistItem(15067) and player:GetPackage():AddItem(15067,1,1) then
			task:AddTaskStep(31204);
			return true;
		end
	end
	return false;
end