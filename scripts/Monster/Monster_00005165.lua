function Monster_00005165(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(191) then
		task:AddTaskStep2(191,1,num);
	end
end