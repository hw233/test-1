function Monster_00005010(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(43) then
		task:AddTaskStep2(43,1,num);
	end
	if task:HasAcceptedTask(609) then
		task:AddTaskStep2(609,1,num);
	end
	if task:HasAcceptedTask(509) then
		task:AddTaskStep2(509,1,num);
	end
end