function Monster_00005180(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(183) then
		task:AddTaskStep2(183,1,num);
	end
end