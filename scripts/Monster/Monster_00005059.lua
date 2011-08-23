function Monster_00005059(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(89) then
		task:AddTaskStep2(89,1,num);
	end
end