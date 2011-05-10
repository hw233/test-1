function Explore_00004248()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(30901) then
		task:AddTaskStep(30901);
		return true;
	end
	return false;
end