function Monster_00005229(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(154) then
		task:AddTaskStep2(154,1,num);
	end
end