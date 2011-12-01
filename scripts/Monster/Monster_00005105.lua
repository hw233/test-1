function Monster_00005105(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(115) then
		task:AddTaskStep2(115,1,num);
	end
end