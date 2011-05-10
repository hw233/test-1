function Monster_00004233(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80049) then
		task:AddTaskStep2(80049,1,num);
	end
	if task:HasAcceptedTask(61201) then
		task:AddTaskStep2(61201,1,num);
	end
	if task:HasAcceptedTask(70307) then
		task:AddTaskStep2(70307,1,num);
	end
	if task:HasAcceptedTask(70317) then
		task:AddTaskStep2(70317,1,num);
	end
end