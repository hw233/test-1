function Monster_00004312(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80076) then
		task:AddTaskStep2(80076,1,num);
	end
	if task:HasAcceptedTask(62103) then
		task:AddTaskStep2(62103,2,num);
	end
end