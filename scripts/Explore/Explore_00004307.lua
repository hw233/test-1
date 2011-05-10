function Explore_00004307()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(32001) then
		task:AddTaskStep(32001);
		return true;
	end
	return false;
end