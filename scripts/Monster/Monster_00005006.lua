function Monster_00005006(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(27) then
		task:AddTaskStep2(27,1,num);
	end
end