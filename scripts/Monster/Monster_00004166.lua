function Monster_00004166(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50602) then
		task:AddTaskStep2(50602,1,num);
	end
end