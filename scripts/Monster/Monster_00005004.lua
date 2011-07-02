function Monster_00005004(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(22) then
		task:AddTaskStep2(22,1,num);
	end
	if task:HasAcceptedTask(104) then
		task:AddTaskStep2(104,1,num);
	end
end