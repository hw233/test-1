function Monster_00004112(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(10105) then
		task:AddTaskStep2(10105,1,num);
	end
	if task:HasAcceptedTask(10106) then
		task:AddTaskStep2(10106,1,num);
	end
end