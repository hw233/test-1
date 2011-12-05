function Monster_00005211(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(148) then
		task:AddTaskStep2(148,1,num);
	end
end