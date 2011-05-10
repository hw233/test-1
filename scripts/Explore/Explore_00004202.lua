function Explore_00004202()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(30301) then
		task:AddTaskStep(30301);
		return true;
	end
	return false;
end