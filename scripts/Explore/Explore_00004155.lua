function Explore_00004155()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(20104) then
		task:AddTaskStep(20104);
		return true;
	end
	return false;
end