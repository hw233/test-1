function Monster_00004188(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50904) then
		task:AddTaskStep2(50904,2,num);
	end
end