function Monster_00005025(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50) then
		task:AddTaskStep2(50,1,num);
	end
end