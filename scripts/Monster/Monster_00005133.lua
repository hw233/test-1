function Monster_00005133(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(153) then
		task:AddTaskStep2(153,1,num);
	end
end