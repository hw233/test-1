function Monster_00004265(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80059) then
		task:AddTaskStep2(80059,1,num);
	end
	if task:HasAcceptedTask(70308) then
		task:AddTaskStep2(70308,1,num);
	end
end