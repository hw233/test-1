function Monster_00005258(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(176) then
		task:AddTaskStep2(176,1,num);
	end
end