function Monster_00005153(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(165) then
		task:AddTaskStep2(165,1,num);
	end
end