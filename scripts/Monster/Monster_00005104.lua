function Monster_00005104(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(114) then
		task:AddTaskStep2(114,1,num);
	end
end