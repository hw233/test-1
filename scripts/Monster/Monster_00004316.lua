function Monster_00004316(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(32005) then
		task:AddTaskStep2(32005,2,num);
	end
end