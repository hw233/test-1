function Monster_00005161(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(189) then
		task:AddTaskStep2(189,1,num);
	end
end