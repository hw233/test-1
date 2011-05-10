function Explore_00004199()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(30202) then
		task:AddTaskStep(30202);
		return true;
	end
	return false;
end