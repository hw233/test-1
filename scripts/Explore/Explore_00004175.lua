function Explore_00004175()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(50701) then
		task:AddTaskStep(50701);
		return true;
	end
	return false;
end