function Monster_00005013(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(54) then
		task:AddTaskStep2(54,1,num);
	end
end