function Monster_00005054(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(79) then
		task:AddTaskStep2(79,1,num);
	end
	if task:HasAcceptedTask(524) then
		task:AddTaskStep2(524,1,num);
	end
	if task:HasAcceptedTask(574) then
		task:AddTaskStep2(574,1,num);
	end
	if task:HasAcceptedTask(619) then
		task:AddTaskStep2(619,1,num);
	end
	if task:HasAcceptedTask(639) then
		task:AddTaskStep2(639,1,num);
	end
	if task:HasAcceptedTask(594) then
		task:AddTaskStep2(594,1,num);
	end
end