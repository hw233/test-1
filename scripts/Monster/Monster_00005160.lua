function Monster_00005160(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(185) then
		task:AddTaskStep2(185,1,num);
	end
end