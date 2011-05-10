function Monster_00004320(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(32005) then
		task:AddTaskStep2(32005,3,num);
	end
end