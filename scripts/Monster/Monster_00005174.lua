function Monster_00005174(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(143) then
		task:AddTaskStep2(143,1,num);
	end
end