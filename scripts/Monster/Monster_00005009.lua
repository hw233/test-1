function Monster_00005009(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(35) then
		task:AddTaskStep2(35,1,num);
	end
	if task:HasAcceptedTask(105) then
		task:AddTaskStep2(105,1,num);
	end
end