function Monster_00004466(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90114) then
		task:AddTaskStep2(90114,1,num);
	end
end