function Monster_00004155(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(20104) then
		task:AddTaskStep2(20104,1,num);
	end
	if task:HasAcceptedTask(20105) then
		task:AddTaskStep2(20105,1,num);
	end
end