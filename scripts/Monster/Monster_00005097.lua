function Monster_00005097(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(104) then
		task:AddTaskStep2(104,1,num);
	end
end