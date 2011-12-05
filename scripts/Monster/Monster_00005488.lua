function Monster_00005488(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(112) then
		task:AddTaskStep2(112,1,num);
	end
end