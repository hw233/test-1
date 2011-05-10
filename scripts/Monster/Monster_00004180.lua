function Monster_00004180(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50802) then
		task:AddTaskStep2(50802,1,num);
	end
end