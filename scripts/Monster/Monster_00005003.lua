function Monster_00005003(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(19) then
		task:AddTaskStep2(19,1,num);
	end
	if task:HasAcceptedTask(502) then
		task:AddTaskStep2(502,1,num);
	end
	if task:HasAcceptedTask(552) then
		task:AddTaskStep2(552,1,num);
	end
	if task:HasAcceptedTask(602) then
		task:AddTaskStep2(602,1,num);
	end
	if task:HasAcceptedTask(623) then
		task:AddTaskStep2(623,1,num);
	end
	if task:HasAcceptedTask(527) then
		task:AddTaskStep2(527,1,num);
	end
	if task:HasAcceptedTask(652) then
		task:AddTaskStep2(652,1,num);
	end
end