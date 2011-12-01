function Monster_00005000(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(2) then
		task:AddTaskStep2(2,1,num);
	end
	if task:HasAcceptedTask(506) then
		task:AddTaskStep2(506,1,num);
	end
	if task:HasAcceptedTask(556) then
		task:AddTaskStep2(556,1,num);
	end
	if task:HasAcceptedTask(606) then
		task:AddTaskStep2(606,1,num);
	end
	if task:HasAcceptedTask(626) then
		task:AddTaskStep2(626,1,num);
	end
	if task:HasAcceptedTask(531) then
		task:AddTaskStep2(531,1,num);
	end
	if task:HasAcceptedTask(576) then
		task:AddTaskStep2(576,1,num);
	end
	if task:HasAcceptedTask(656) then
		task:AddTaskStep2(656,1,num);
	end
end