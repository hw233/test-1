function Monster_00005100(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(108) then
		task:AddTaskStep2(108,1,num);
	end
end