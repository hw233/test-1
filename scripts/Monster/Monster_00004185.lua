function Monster_00004185(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50903) then
		task:AddTaskStep2(50903,1,num);
	end
	if task:HasAcceptedTask(70304) then
		task:AddTaskStep2(70304,1,num);
	end
end