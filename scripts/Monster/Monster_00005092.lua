function Monster_00005092(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(93) then
		task:AddTaskStep2(93,1,num);
	end
end