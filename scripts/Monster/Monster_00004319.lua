function Monster_00004319(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80078) then
		task:AddTaskStep2(80078,1,num);
	end
	if task:HasAcceptedTask(62203) then
		task:AddTaskStep2(62203,1,num);
	end
end