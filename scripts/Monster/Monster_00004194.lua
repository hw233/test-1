function Monster_00004194(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30106) then
		task:AddTaskStep2(30106,1,num);
	end
	if task:HasAcceptedTask(60102) then
		task:AddTaskStep2(60102,1,num);
	end
end