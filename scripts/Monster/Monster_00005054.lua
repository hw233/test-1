function Monster_00005054(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(79) then
		task:AddTaskStep2(79,1,num);
	end
end