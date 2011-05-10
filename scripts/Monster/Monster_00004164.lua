function Monster_00004164(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50302) then
		task:AddTaskStep2(50302,1,num);
	end
end