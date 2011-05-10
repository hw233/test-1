function Monster_00004315(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80077) then
		task:AddTaskStep2(80077,1,num);
	end
end