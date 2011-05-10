function Monster_00004278(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80062) then
		task:AddTaskStep2(80062,1,num);
	end
end