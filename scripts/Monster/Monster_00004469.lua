function Monster_00004469(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80704) then
		task:AddTaskStep2(80704,1,num);
	end
end