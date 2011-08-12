function Monster_00005001(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(607) then
		task:AddTaskStep2(607,1,num);
	end
	if task:HasAcceptedTask(507) then
		task:AddTaskStep2(507,1,num);
	end
	if task:HasAcceptedTask(1) then
		task:AddTaskStep2(1,1,num);
	end
end