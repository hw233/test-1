function Monster_00005095(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(98) then
		task:AddTaskStep2(98,1,num);
	end
end