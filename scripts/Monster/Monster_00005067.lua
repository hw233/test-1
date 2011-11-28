function Monster_00005067(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(122) then
		task:AddTaskStep2(122,1,num);
	end
end