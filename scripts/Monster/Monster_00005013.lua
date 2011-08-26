function Monster_00005013(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(54) then
		task:AddTaskStep2(54,1,num);
	end
	if task:HasAcceptedTask(611) then
		task:AddTaskStep2(611,1,num);
	end
	if task:HasAcceptedTask(520) then
		task:AddTaskStep2(520,1,num);
	end
	if task:HasAcceptedTask(570) then
		task:AddTaskStep2(570,1,num);
	end
end