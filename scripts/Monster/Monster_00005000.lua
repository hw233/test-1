function Monster_00005000(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(2) then
		task:AddTaskStep2(2,1,num);
	end
end