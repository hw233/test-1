function Monster_00004308(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(70310) then
		task:AddTaskStep2(70310,1,num);
	end
end