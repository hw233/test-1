function Monster_00005098(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(106) then
		task:AddTaskStep2(106,1,num);
	end
end