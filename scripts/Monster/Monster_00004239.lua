function Monster_00004239(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30806) then
		task:AddTaskStep2(30806,1,num);
	end
end