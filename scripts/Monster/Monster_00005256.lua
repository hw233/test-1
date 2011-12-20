function Monster_00005256(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(174) then
		task:AddTaskStep2(174,1,num);
	end
end