function Monster_00005151(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(171) then
		task:AddTaskStep2(171,1,num);
	end
end