function Monster_00004193(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30102) then
		task:AddTaskStep2(30102,1,num);
	end
	if task:HasAcceptedTask(30103) then
		task:AddTaskStep2(30103,1,num);
	end
end