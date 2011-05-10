function Monster_00004132(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40802) then
		task:AddTaskStep2(40802,1,num);
	end
	if task:HasAcceptedTask(70301) then
		task:AddTaskStep2(70301,1,num);
	end
end