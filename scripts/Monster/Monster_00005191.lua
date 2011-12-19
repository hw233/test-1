function Monster_00005191(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(187) then
		task:AddTaskStep2(187,1,num);
	end
end