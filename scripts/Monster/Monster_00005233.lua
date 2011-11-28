function Monster_00005233(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(157) then
		task:AddTaskStep2(157,1,num);
	end
end