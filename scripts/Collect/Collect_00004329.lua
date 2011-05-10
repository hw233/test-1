function Collect_00004329()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(10404) then
		if not player:GetPackage():ExistItem(15026) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15026);
		end
	end
	return action;
end



function Collect_00004329_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(10404) then
		if not player:GetPackage():ExistItem(15026) and player:GetPackage():AddItem(15026,1,1) then
			task:AddTaskStep(10404);
			return true;
		end
	end
	return false;
end