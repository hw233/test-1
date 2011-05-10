function Collect_00004096()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(90104) then
		if not player:GetPackage():ExistItem(15103) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15103);
		end
	end
	return action;
end



function Collect_00004096_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(90104) then
		if not player:GetPackage():ExistItem(15103) and player:GetPackage():AddItem(15103,1,1) then
			task:AddTaskStep(90104);
			return true;
		end
	end
	return false;
end