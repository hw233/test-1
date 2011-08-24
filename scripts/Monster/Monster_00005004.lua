function Monster_00005004(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(654) then
		task:AddTaskStep2(654,1,num);
	end
	if task:HasAcceptedTask(504) then
		task:AddTaskStep2(504,1,num);
	end
	if task:HasAcceptedTask(554) then
		task:AddTaskStep2(554,1,num);
	end
	if task:HasAcceptedTask(604) then
		task:AddTaskStep2(604,1,num);
	end
	if task:HasAcceptedTask(22) then
		task:AddTaskStep2(22,1,num);
	end
end