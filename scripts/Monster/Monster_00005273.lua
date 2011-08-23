function Monster_00005273(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(88) then
		task:AddTaskStep2(88,1,num);
	end
end