function Monster_00004175(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50701) then
		task:AddTaskStep2(50701,1,num);
	end
	if task:HasAcceptedTask(50702) then
		task:AddTaskStep2(50702,1,num);
	end
end