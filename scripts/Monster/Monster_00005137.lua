function Monster_00005137(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(152) then
		task:AddTaskStep2(152,1,num);
	end
end