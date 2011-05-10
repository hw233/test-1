function Monster_00004353(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80042) then
		task:AddTaskStep2(80042,1,num);
	end
end