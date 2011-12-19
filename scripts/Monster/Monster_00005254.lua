function Monster_00005254(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(172) then
		task:AddTaskStep2(172,1,num);
	end
end