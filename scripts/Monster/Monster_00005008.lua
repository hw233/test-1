function Monster_00005008(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(29) then
		task:AddTaskStep2(29,1,num);
	end
end