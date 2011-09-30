function Monster_00005005(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(24) then
		task:AddTaskStep2(24,1,num);
	end
	if task:HasAcceptedTask(508) then
		task:AddTaskStep2(508,1,num);
	end
	if task:HasAcceptedTask(558) then
		task:AddTaskStep2(558,1,num);
	end
	if task:HasAcceptedTask(608) then
		task:AddTaskStep2(608,1,num);
	end
	if task:HasAcceptedTask(628) then
		task:AddTaskStep2(628,1,num);
	end
	if task:HasAcceptedTask(533) then
		task:AddTaskStep2(533,1,num);
	end
	if task:HasAcceptedTask(578) then
		task:AddTaskStep2(578,1,num);
	end
end