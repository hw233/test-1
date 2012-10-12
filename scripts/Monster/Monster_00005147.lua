function Monster_00005147(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(162) then
		task:AddTaskStep2(162,1,num);
	end
end