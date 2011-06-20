function Monster_00005020(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(61) then
		task:AddTaskStep2(61,1,num);
	end
end