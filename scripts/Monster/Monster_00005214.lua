function Monster_00005214(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(145) then
		task:AddTaskStep2(145,1,num);
	end
end