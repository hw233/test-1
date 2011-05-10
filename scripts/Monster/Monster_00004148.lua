function Monster_00004148(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(20103) then
		task:AddTaskStep2(20103,1,num);
	end
end