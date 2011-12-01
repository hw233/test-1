function Monster_00005230(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(155) then
		task:AddTaskStep2(155,1,num);
	end
end