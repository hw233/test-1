function Monster_00004137(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40903) then
		task:AddTaskStep2(40903,1,num);
	end
	if task:HasAcceptedTask(70302) then
		task:AddTaskStep2(70302,1,num);
	end
end