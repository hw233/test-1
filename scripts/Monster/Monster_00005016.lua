function Monster_00005016(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(56) then
		task:AddTaskStep2(56,1,num);
	end
end