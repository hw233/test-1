function Monster_00005053(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(77) then
		task:AddTaskStep2(77,1,num);
	end
end