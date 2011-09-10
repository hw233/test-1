function Monster_00005011(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(44) then
		task:AddTaskStep2(44,1,num);
	end
	if task:HasAcceptedTask(503) then
		task:AddTaskStep2(503,1,num);
	end
	if task:HasAcceptedTask(553) then
		task:AddTaskStep2(553,1,num);
	end
	if task:HasAcceptedTask(603) then
		task:AddTaskStep2(603,1,num);
	end
	if task:HasAcceptedTask(653) then
		task:AddTaskStep2(653,1,num);
	end
end