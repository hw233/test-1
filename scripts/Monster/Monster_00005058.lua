function Monster_00005058(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(87) then
		task:AddTaskStep2(87,1,num);
	end
end