function Monster_00004302(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80074) then
		task:AddTaskStep2(80074,1,num);
	end
end