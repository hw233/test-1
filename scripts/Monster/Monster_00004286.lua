function Monster_00004286(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(31406) then
		task:AddTaskStep2(31406,1,num);
	end
end