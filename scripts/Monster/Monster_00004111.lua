function Monster_00004111(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(10104) then
		task:AddTaskStep2(10104,1,num);
	end
end