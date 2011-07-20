function Monster_00005012(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(46) then
		task:AddTaskStep2(46,1,num);
	end
	if task:HasAcceptedTask(510) then
		task:AddTaskStep2(510,1,num);
	end
	if task:HasAcceptedTask(610) then
		task:AddTaskStep2(610,1,num);
	end
end