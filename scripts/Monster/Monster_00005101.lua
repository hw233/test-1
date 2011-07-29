function Monster_00005101(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(110) then
		task:AddTaskStep2(110,1,num);
	end
end