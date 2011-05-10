function Monster_00004363(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80070) then
		task:AddTaskStep2(80070,1,num);
	end
end