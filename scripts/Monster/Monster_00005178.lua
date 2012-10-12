function Monster_00005178(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(141) then
		task:AddTaskStep2(141,1,num);
	end
end