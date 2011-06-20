function Monster_00005017(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(57) then
		task:AddTaskStep2(57,1,num);
	end
end