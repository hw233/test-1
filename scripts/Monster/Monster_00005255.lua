function Monster_00005255(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(173) then
		task:AddTaskStep2(173,1,num);
	end
end