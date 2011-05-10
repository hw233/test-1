function Monster_00004360(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80041) then
		task:AddTaskStep2(80041,1,num);
	end
end