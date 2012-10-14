function Monster_00005176(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(142) then
		task:AddTaskStep2(142,1,num);
	end
end