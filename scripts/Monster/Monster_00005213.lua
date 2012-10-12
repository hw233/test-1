function Monster_00005213(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(146) then
		task:AddTaskStep2(146,1,num);
	end
end