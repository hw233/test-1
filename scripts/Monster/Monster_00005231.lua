function Monster_00005231(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(156) then
		task:AddTaskStep2(156,1,num);
	end
end