function Monster_00005293(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(109) then
		task:AddTaskStep2(109,1,num);
	end
end