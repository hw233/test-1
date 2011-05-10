function Monster_00004222(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80044) then
		task:AddTaskStep2(80044,1,num);
	end
	if task:HasAcceptedTask(60602) then
		task:AddTaskStep2(60602,1,num);
	end
	if task:HasAcceptedTask(30409) then
		task:AddTaskStep2(30409,1,num);
	end
end