function Monster_00005132(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(150) then
		task:AddTaskStep2(150,1,num);
	end
end