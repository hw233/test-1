function Monster_00005257(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(175) then
		task:AddTaskStep2(175,1,num);
	end
end