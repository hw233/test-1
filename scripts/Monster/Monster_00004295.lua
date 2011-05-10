function Monster_00004295(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(31603) then
		task:AddTaskStep2(31603,1,num);
	end
end