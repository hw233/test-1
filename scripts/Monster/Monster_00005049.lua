function Monster_00005049(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(68) then
		task:AddTaskStep2(68,1,num);
	end
end