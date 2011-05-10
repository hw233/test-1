function Monster_00004167(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50502) then
		task:AddTaskStep2(50502,1,num);
	end
end