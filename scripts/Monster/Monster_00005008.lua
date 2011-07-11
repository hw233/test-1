function Monster_00005008(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(37) then
		task:AddTaskStep2(37,1,num);
	end
	if task:HasAcceptedTask(100) then
		task:AddTaskStep2(100,1,num);
	end
	if task:HasAcceptedTask(29) then
		task:AddTaskStep2(29,1,num);
	end
end