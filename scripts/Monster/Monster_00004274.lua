function Monster_00004274(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80064) then
		task:AddTaskStep2(80064,1,num);
	end
end