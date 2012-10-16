function Monster_00005063(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(37) then
		task:AddTaskStep2(37,1,num);
	end
end