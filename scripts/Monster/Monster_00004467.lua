function Monster_00004467(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90116) then
		task:AddTaskStep2(90116,1,num);
	end
end