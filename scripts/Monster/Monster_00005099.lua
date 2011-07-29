function Monster_00005099(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(107) then
		task:AddTaskStep2(107,1,num);
	end
end