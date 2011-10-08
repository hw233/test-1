function Monster_00005022(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(63) then
		task:AddTaskStep2(63,1,num);
	end
	if task:HasAcceptedTask(522) then
		task:AddTaskStep2(522,1,num);
	end
	if task:HasAcceptedTask(572) then
		task:AddTaskStep2(572,1,num);
	end
	if task:HasAcceptedTask(617) then
		task:AddTaskStep2(617,1,num);
	end
	if task:HasAcceptedTask(637) then
		task:AddTaskStep2(637,1,num);
	end
	if task:HasAcceptedTask(592) then
		task:AddTaskStep2(592,1,num);
	end
end