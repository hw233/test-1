function Monster_00005000(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(506) then
		task:AddTaskStep2(506,1,num);
	end
	if task:HasAcceptedTask(606) then
		task:AddTaskStep2(606,1,num);
	end
	if task:HasAcceptedTask(2) then
		task:AddTaskStep2(2,1,num);
	end
end