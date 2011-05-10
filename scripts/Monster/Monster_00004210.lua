function Monster_00004210(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(60401) then
		task:AddTaskStep2(60401,1,num);
	end
	if task:HasAcceptedTask(70306) then
		task:AddTaskStep2(70306,1,num);
	end
	if task:HasAcceptedTask(80040) then
		task:AddTaskStep2(80040,1,num);
	end
end