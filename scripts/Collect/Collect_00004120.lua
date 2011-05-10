function Collect_00004120()
	local player = GetPlayer();
	local action = ActionTable:Instance();
	if player:GetTaskMgr():HasAcceptedTask(40603) then
		if not player:GetPackage():ExistItem(15052) then
			action.m_ActionType = 0x0002;
			action.m_ActionToken = 3;
			action.m_ActionStep = 01;
			action.m_ActionMsg = "采集" .. GetItemName(15052);
		end
	end
	return action;
end



function Collect_00004120_Item_Action()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(40603) then
		if not player:GetPackage():ExistItem(15052) and player:GetPackage():AddItem(15052,1,1) then
			task:AddTaskStep(40603);
			return true;
		end
	end
	return false;
end