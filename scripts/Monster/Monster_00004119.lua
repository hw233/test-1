function Monster_00004119(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40402) then
		task:AddTaskStep2(40402,1,num);
	end
end