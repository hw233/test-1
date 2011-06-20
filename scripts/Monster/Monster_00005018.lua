function Monster_00005018(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(58) then
		task:AddTaskStep2(58,1,num);
	end
end