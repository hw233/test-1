function Monster_00005005(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(24) then
		task:AddTaskStep2(24,1,num);
	end
end