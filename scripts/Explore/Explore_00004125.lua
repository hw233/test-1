function Explore_00004125()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(40701) then
		task:AddTaskStep(40701);
		return true;
	end
	return false;
end