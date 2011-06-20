function Monster_00005022(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(63) then
		task:AddTaskStep2(63,1,num);
	end
end