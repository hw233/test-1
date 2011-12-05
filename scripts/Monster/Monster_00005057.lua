function Monster_00005057(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(86) then
		task:AddTaskStep2(86,1,num);
	end
end