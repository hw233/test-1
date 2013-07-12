function Monster_00005410(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(200) then
		task:AddTaskStep2(200,1,num);
	end
end