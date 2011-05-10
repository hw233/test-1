function Monster_00004200(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30206) then
		task:AddTaskStep2(30206,1,num);
	end
	if task:HasAcceptedTask(60202) then
		task:AddTaskStep2(60202,1,num);
	end
end