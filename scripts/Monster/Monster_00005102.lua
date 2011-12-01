function Monster_00005102(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(111) then
		task:AddTaskStep2(111,1,num);
	end
end