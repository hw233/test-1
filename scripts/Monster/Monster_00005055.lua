function Monster_00005055(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80) then
		task:AddTaskStep2(80,1,num);
	end
end