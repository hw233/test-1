function Monster_00005088(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90) then
		task:AddTaskStep2(90,1,num);
	end
end