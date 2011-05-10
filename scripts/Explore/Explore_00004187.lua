function Explore_00004187()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(20501) then
		task:AddTaskStep(20501);
		return true;
	end
	return false;
end