function Monster_00004299(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80071) then
		task:AddTaskStep2(80071,1,num);
	end
end