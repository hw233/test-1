function Monster_00004468(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(90120) then
		task:AddTaskStep2(90120,1,num);
	end
end