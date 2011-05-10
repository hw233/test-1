function Monster_00004259(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80057) then
		task:AddTaskStep2(80057,1,num);
	end
	if task:HasAcceptedTask(61501) then
		task:AddTaskStep2(61501,1,num);
	end
end