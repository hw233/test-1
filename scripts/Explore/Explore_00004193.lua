function Explore_00004193()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(30102) then
		task:AddTaskStep(30102);
		return true;
	end
	return false;
end