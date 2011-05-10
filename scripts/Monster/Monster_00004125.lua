function Monster_00004125(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40702) then
		task:AddTaskStep2(40702,1,num);
	end
	if task:HasAcceptedTask(40701) then
		task:AddTaskStep2(40701,1,num);
	end
end