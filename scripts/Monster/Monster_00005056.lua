function Monster_00005056(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(525) then
		task:AddTaskStep2(525,1,num);
	end
	if task:HasAcceptedTask(575) then
		task:AddTaskStep2(575,1,num);
	end
	if task:HasAcceptedTask(620) then
		task:AddTaskStep2(620,1,num);
	end
	if task:HasAcceptedTask(595) then
		task:AddTaskStep2(595,1,num);
	end
	if task:HasAcceptedTask(81) then
		task:AddTaskStep2(81,1,num);
	end
end