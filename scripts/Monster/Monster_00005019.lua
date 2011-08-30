function Monster_00005019(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(60) then
		task:AddTaskStep2(60,1,num);
	end
	if task:HasAcceptedTask(516) then
		task:AddTaskStep2(516,1,num);
	end
	if task:HasAcceptedTask(566) then
		task:AddTaskStep2(566,1,num);
	end
	if task:HasAcceptedTask(615) then
		task:AddTaskStep2(615,1,num);
	end
end