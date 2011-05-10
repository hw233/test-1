function Monster_00004263(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(61603) then
		task:AddTaskStep2(61603,1,num);
	end
	if task:HasAcceptedTask(31205) then
		task:AddTaskStep2(31205,1,num);
	end
end