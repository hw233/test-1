function Monster_00004126(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(10307) then
		task:AddTaskStep2(10307,1,num);
	end
	if task:HasAcceptedTask(10308) then
		task:AddTaskStep2(10308,1,num);
	end
end