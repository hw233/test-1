function Monster_00004199(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30202) then
		task:AddTaskStep2(30202,1,num);
	end
	if task:HasAcceptedTask(30203) then
		task:AddTaskStep2(30203,1,num);
	end
end