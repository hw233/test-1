function Monster_00004140(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40904) then
		task:AddTaskStep2(40904,1,num);
	end
end