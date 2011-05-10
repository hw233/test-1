function Monster_00004476(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90105) then
		task:AddTaskStep2(90105,1,num);
	end
end