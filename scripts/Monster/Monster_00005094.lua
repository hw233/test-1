function Monster_00005094(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(97) then
		task:AddTaskStep2(97,1,num);
	end
end