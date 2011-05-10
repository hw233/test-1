function Monster_00004179(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(70303) then
		task:AddTaskStep2(70303,1,num);
	end
end