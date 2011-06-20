function Monster_00005002(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(12) then
		task:AddTaskStep2(12,1,num);
	end
end