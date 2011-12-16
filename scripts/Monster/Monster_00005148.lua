function Monster_00005148(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(169) then
		task:AddTaskStep2(169,1,num);
	end
end