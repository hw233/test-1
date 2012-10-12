function Monster_00005150(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(168) then
		task:AddTaskStep2(168,1,num);
	end
end