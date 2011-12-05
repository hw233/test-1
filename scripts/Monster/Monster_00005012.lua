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
	if task:HasAcceptedTask(560) then
		task:AddTaskStep2(560,1,num);
	end
	if task:HasAcceptedTask(610) then
		task:AddTaskStep2(610,1,num);
	end
	if task:HasAcceptedTask(630) then
		task:AddTaskStep2(630,1,num);
	end
	if task:HasAcceptedTask(535) then
		task:AddTaskStep2(535,1,num);
	end
	if task:HasAcceptedTask(580) then
		task:AddTaskStep2(580,1,num);
	end
	if task:HasAcceptedTask(657) then
		task:AddTaskStep2(657,1,num);
	end
end