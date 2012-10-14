function Monster_00005182(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(178) then
		task:AddTaskStep2(178,1,num);
	end
end