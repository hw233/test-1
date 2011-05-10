function Monster_00004290(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80068) then
		task:AddTaskStep2(80068,1,num);
	end
	if task:HasAcceptedTask(62002) then
		task:AddTaskStep2(62002,1,num);
	end
end