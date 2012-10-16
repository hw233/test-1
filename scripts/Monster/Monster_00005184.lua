function Monster_00005184(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(180) then
		task:AddTaskStep2(180,1,num);
	end
end