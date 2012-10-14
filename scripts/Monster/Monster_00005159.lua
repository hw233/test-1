function Monster_00005159(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(186) then
		task:AddTaskStep2(186,1,num);
	end
end