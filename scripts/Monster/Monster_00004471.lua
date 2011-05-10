function Monster_00004471(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90121) then
		task:AddTaskStep2(90121,1,num);
	end
	if task:HasAcceptedTask(90124) then
		task:AddTaskStep2(90124,1,num);
	end
end