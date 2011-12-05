function Monster_00005138(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(159) then
		task:AddTaskStep2(159,1,num);
	end
end