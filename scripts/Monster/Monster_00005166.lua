function Monster_00005166(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(192) then
		task:AddTaskStep2(192,1,num);
	end
end