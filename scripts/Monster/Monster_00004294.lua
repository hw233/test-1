function Monster_00004294(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(31703) then
		task:AddTaskStep2(31703,1,num);
	end
end