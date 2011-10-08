function Monster_00005053(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(77) then
		task:AddTaskStep2(77,1,num);
	end
	if task:HasAcceptedTask(523) then
		task:AddTaskStep2(523,1,num);
	end
	if task:HasAcceptedTask(573) then
		task:AddTaskStep2(573,1,num);
	end
	if task:HasAcceptedTask(618) then
		task:AddTaskStep2(618,1,num);
	end
	if task:HasAcceptedTask(638) then
		task:AddTaskStep2(638,1,num);
	end
	if task:HasAcceptedTask(593) then
		task:AddTaskStep2(593,1,num);
	end
end