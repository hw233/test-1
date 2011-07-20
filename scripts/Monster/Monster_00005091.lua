function Monster_00005091(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(92) then
		task:AddTaskStep2(92,1,num);
	end
end