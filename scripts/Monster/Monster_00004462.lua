function Monster_00004462(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90103) then
		task:AddTaskStep2(90103,1,num);
	end
end