function Monster_00005019(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(60) then
		task:AddTaskStep2(60,1,num);
	end
end