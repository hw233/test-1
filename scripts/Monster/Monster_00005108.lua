function Monster_00005108(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(119) then
		task:AddTaskStep2(119,1,num);
	end
end