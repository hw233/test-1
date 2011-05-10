function Monster_00004465(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90113) then
		task:AddTaskStep2(90113,1,num);
	end
end