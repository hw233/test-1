function Monster_00004105(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(10103) then
		task:AddTaskStep2(10103,1,num);
	end
end