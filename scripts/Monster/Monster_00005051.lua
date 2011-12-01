function Monster_00005051(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(521) then
		task:AddTaskStep2(521,1,num);
	end
	if task:HasAcceptedTask(571) then
		task:AddTaskStep2(571,1,num);
	end
	if task:HasAcceptedTask(616) then
		task:AddTaskStep2(616,1,num);
	end
	if task:HasAcceptedTask(636) then
		task:AddTaskStep2(636,1,num);
	end
	if task:HasAcceptedTask(591) then
		task:AddTaskStep2(591,1,num);
	end
	if task:HasAcceptedTask(74) then
		task:AddTaskStep2(74,1,num);
	end
end