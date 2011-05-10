function Monster_00004289(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(31504) then
		task:AddTaskStep2(31504,1,num);
	end
end