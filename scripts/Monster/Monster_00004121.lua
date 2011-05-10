function Monster_00004121(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40602) then
		task:AddTaskStep2(40602,1,num);
	end
end