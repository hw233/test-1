function Monster_00004461(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80502) then
		task:AddTaskStep2(80502,1,num);
	end
	if task:HasAcceptedTask(80503) then
		task:AddTaskStep2(80503,1,num);
	end
end