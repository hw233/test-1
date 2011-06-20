function Monster_00005032(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(49) then
		task:AddTaskStep2(49,1,num);
	end
end