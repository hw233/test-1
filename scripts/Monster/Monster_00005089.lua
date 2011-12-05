function Monster_00005089(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(91) then
		task:AddTaskStep2(91,1,num);
	end
end