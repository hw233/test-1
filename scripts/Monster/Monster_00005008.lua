function Monster_00005008(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(29) then
		task:AddTaskStep2(29,1,num);
	end
	if task:HasAcceptedTask(500) then
		task:AddTaskStep2(500,1,num);
	end
	if task:HasAcceptedTask(550) then
		task:AddTaskStep2(550,1,num);
	end
	if task:HasAcceptedTask(600) then
		task:AddTaskStep2(600,1,num);
	end
	if task:HasAcceptedTask(650) then
		task:AddTaskStep2(650,1,num);
	end
end