function Monster_00005131(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(166) then
		task:AddTaskStep2(166,1,num);
	end
end