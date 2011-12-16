function Monster_00005183(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(179) then
		task:AddTaskStep2(179,1,num);
	end
end