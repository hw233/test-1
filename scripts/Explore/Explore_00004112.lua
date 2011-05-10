function Explore_00004112()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(10105) then
		task:AddTaskStep(10105);
		return true;
	end
	return false;
end