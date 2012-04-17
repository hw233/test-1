function Monster_00005167(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(194) then
		task:AddTaskStep2(194,1,num);
	end
end