function Monster_00004304(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80073) then
		task:AddTaskStep2(80073,1,num);
	end
	if task:HasAcceptedTask(62102) then
		task:AddTaskStep2(62102,1,num);
	end
end