function Monster_00005212(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(147) then
		task:AddTaskStep2(147,1,num);
	end
end