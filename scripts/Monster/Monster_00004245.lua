function Monster_00004245(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30707) then
		task:AddTaskStep2(30707,1,num);
	end
	if task:HasAcceptedTask(61304) then
		task:AddTaskStep2(61304,1,num);
	end
end