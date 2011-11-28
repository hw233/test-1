function Monster_00005096(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(102) then
		task:AddTaskStep2(102,1,num);
	end
end