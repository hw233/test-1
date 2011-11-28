function Monster_00005021(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(62) then
		task:AddTaskStep2(62,1,num);
	end
end