function Monster_00005093(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(96) then
		task:AddTaskStep2(96,1,num);
	end
end