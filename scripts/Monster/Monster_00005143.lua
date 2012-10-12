function Monster_00005143(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(160) then
		task:AddTaskStep2(160,1,num);
	end
end