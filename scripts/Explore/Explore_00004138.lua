function Explore_00004138()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(10501) then
		task:AddTaskStep(10501);
		return true;
	end
	return false;
end