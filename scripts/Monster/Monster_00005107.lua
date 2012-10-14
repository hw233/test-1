function Monster_00005107(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(118) then
		task:AddTaskStep2(118,1,num);
	end
end