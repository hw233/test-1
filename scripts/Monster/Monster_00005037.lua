function Monster_00005037(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(121) then
		task:AddTaskStep2(121,1,num);
	end
end