function Monster_00004130(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40502) then
		task:AddTaskStep2(40502,1,num);
	end
end