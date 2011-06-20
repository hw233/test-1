function Monster_00005003(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(19) then
		task:AddTaskStep2(19,1,num);
	end
end