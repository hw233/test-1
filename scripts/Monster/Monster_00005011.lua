function Monster_00005011(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(44) then
		task:AddTaskStep2(44,1,num);
	end
	if task:HasAcceptedTask(103) then
		task:AddTaskStep2(103,1,num);
	end
end