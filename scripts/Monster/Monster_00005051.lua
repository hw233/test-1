function Monster_00005051(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(74) then
		task:AddTaskStep2(74,1,num);
	end
end