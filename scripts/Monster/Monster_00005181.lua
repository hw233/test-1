function Monster_00005181(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(182) then
		task:AddTaskStep2(182,1,num);
	end
end