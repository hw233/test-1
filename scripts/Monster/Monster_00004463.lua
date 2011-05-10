function Monster_00004463(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90107) then
		task:AddTaskStep2(90107,1,num);
	end
	if task:HasAcceptedTask(90108) then
		task:AddTaskStep2(90108,1,num);
	end
end