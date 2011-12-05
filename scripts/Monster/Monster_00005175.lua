function Monster_00005175(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(144) then
		task:AddTaskStep2(144,1,num);
	end
end