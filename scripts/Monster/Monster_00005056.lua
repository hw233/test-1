function Monster_00005056(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(81) then
		task:AddTaskStep2(81,1,num);
	end
end